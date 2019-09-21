/**
 *   @defgroup  eMPL
 *   @brief     Embedded Motion Processing Library
 *
 *   @{
 *       @file      motion_driver_test.c
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//开启调试信息输出
#define   DEBUG_ON_LOG

#ifdef  DEBUG_ON_LOG
#define MPL_LOGI(...)      do { printf("<<MPU6050--Log>@File:%s@Line:%d@FUNC:%s""", __FILE__, __LINE__, __FUNCTION__);printf(__VA_ARGS__);} while(0);
#define MPL_LOGE(...)      do { printf("<<MPU6050--Log>@File:%s@Line:%d@FUNC:%s""", __FILE__, __LINE__, __FUNCTION__);printf(__VA_ARGS__);} while(0);
#else
#define MPL_LOGI(...)      do { } while(0);
#define MPL_LOGE(...)      do { } while(0);
#endif

#include "mpu_dmp_useapi.h"
#include "stm32f4xx.h"
#include "./i2c/i2c.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/* Data requested by client. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)

#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (100)

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)

struct rx_s {
    unsigned char header[3];
    unsigned char cmd;
};
struct hal_s {
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned short report;
    unsigned short dmp_features;
    unsigned char motion_int_mode;
    struct rx_s rx;
};
static struct hal_s hal = {0};

/* USB RX binary semaphore. Actually, it's just a flag. Not included in struct
 * because it's declared extern elsewhere.
 */
volatile unsigned char rx_new;

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};


/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static inline unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static inline unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}





//重启系统
void SYSTEM_Reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

static inline void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        /*accel_sens = 0;   // 此句可实现不校准  即以初始位置为参考点*/
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
    }

}


/* Every time new gyro data is available, this function is called in an
 * ISR context. In this example, it sets a flag protecting the FIFO read
 * function.
 */
void gyro_data_ready_cb(void)
{
    hal.new_gyro = 1;
}


/**
 * @brief  初始化MPU6050
 * @return 0 if success
 */
u8 mpu_dmp_init(void)
{
    int result;

    result = mpu_init();
    if (result) {
        SYSTEM_Reset();
        return 1;
    }

    /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
    if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {
        MPL_LOGE("error.\n");
        return 2;
    }
    /* Push both gyro and accel data into the FIFO. */
    if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {
        MPL_LOGE("error.\n");
        return 3;
    }
    if (mpu_set_sample_rate(DEFAULT_MPU_HZ)) {
        MPL_LOGE("error.\n");
        return 4;
    }

    /* Initialize HAL state variables. */
    memset(&hal, 0, sizeof(hal));
    hal.sensors = ACCEL_ON | GYRO_ON;

    /* To initialize the DMP:
     * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
     *    inv_mpu_dmp_motion_driver.h into the MPU memory.
     * 2. Push the gyro and accel orientation matrix to the DMP.
     * 3. Register gesture callbacks. Don't worry, these callbacks won't be
     *    executed unless the corresponding feature is enabled.
     * 4. Call dmp_enable_feature(mask) to enable different features.
     * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
     * 6. Call any feature-specific control functions.
     *
     * To enable the DMP, just call mpu_set_dmp_state(1). This function can
     * be called repeatedly to enable and disable the DMP at runtime.
     *
     * The following is a short summary of the features supported in the DMP
     * image provided in inv_mpu_dmp_motion_driver.c:
     * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
     * 200Hz. Integrating the gyro data at higher rates reduces numerical
     * errors (compared to integration on the MCU at a lower sampling rate).
     * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
     * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
     * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
     * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
     * an event at the four orientations where the screen should rotate.
     * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
     * no motion.
     * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
     * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
     * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
     * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
     */
    if (dmp_load_motion_driver_firmware()) {
        MPL_LOGE("error.\n");
        return 5;
    }
    if (dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)) ) {
        MPL_LOGE("error.\n");
        return 6;
    }
        
    hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
    if (dmp_enable_feature(hal.dmp_features)) {
        MPL_LOGE("error.\n");
        return 7;
    }
    if (dmp_set_fifo_rate(DEFAULT_MPU_HZ)) {
        MPL_LOGE("error.\n");
        return 8;
    }
    mpu_set_dmp_state(1);
    hal.dmp_on = 1;

    return 0;
}

/*下列全局变量可供外部使用 */
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];

#define q30     1073741824.0f
float q0=1.0f, q1=0.0f, q2=0.0f, q3=0.0f;
float pitch, roll, yaw;
float gyroX, gyroY, gyroZ;

/**
 * @brief  获取数据供外部使用, 需定时调用
 * @return 0 if success
 */
u8 dmp_get_data (void)
{
    unsigned long sensor_timestamp;

    if (hal.new_gyro && hal.dmp_on) {      
        /* This function gets new data from the FIFO when the DMP is in
         * use. The FIFO can contain any combination of gyro, accel,
         * quaternion, and gesture data. The sensors parameter tells the
         * caller which data fields were actually populated with new data.
         * For example, if sensors == (INV_XYZ_GYRO | INV_WXYZ_QUAT), then
         * the FIFO isn't being filled with accel data.
         * The driver parses the gesture data to determine if a gesture
         * event has occurred; on an event, the application will be notified
         * via a callback (assuming that a callback function was properly
         * registered). The more parameter is non-zero if there are
         * leftover packets in the FIFO.
         */
        if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more)) {
            MPL_LOGE("error.\n");
            return 1;
        }
        if (!more)
            hal.new_gyro = 0;
    }
    
    if (sensors & INV_WXYZ_QUAT) {
        q0 = quat[0] / q30;
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;           
        pitch = asin(-2*q1*q3 + 2*q0*q2) * 57.3;
        roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 57.3;
        yaw = atan2(2*(q1*q2+q0*q3), q0*q0 + q1*q1 - q2*q2 - q3*q3) * 57.3;  
        
//        printf("Pitch:%0.4f  Roll:%0.4f  Yaw:%0.4f\r\n",pitch, roll, yaw);
        gyroX= gyro[0];
        gyroY= gyro[1];
        gyroZ= gyro[2];
    } 
    
    return 0;
}


