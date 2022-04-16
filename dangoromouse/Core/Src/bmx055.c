#include "bmx055.h"
#include "stm32f4xx_hal.h"
extern SPI_HandleTypeDef hspi1;

static bool flag_gyro = false;
float gz;
float gz_offset;

uint8_t read_byte(uint8_t reg)
{
    uint8_t rx_data[2];
    uint8_t tx_data[2];

    tx_data[0] = reg | 0x80;
    tx_data[1] = 0x00; // dummy

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);

    return rx_data[1];
}

void write_byte(uint8_t reg, uint8_t data)
{
    uint8_t rx_data[2];
    uint8_t tx_data[2];

    tx_data[0] = reg & 0x7F;
    //   tx_data[0] = reg | 0x00;
    tx_data[1] = data; // write data

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET); // CSピン立ち下げ
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET); // CSピン立ち上げ
}

void GyroInit()
{
    uint8_t who_am_i;

    HAL_Delay(100);                          // wait start up
    who_am_i = read_byte(GYRO_WHOAMI);          // read who am i
    printf("who_am_i = 0x%x\r\n", who_am_i); // check who am i value
    HAL_Delay(10);
    while (flag_gyro == false)
    {
        who_am_i = read_byte(GYRO_WHOAMI);
        if (who_am_i == 0x0F)
        {
            flag_gyro = true;
            printf("who_am_i = 0x%x\r\n", who_am_i);
        }
        // error check
        else
        {
            printf("who_am_i = 0x%x\r\n", who_am_i);
            HAL_Delay(10);
            // printf("gyro_error \r\n");
        }
    }

//    HAL_Delay(50);
//    write_byte(PWR_MGMT_1, 0x00); // set pwr_might (20MHz)
//    HAL_Delay(50);
//    write_byte(CONFIG, 0x00); // set config (FSYNCはNC)
//    HAL_Delay(50);
//    write_byte(GYRO_CONFIG, 0x18); // set gyro config (2000dps)
//    HAL_Delay(50);
}

void GyroOffsetCalc()
{
    int16_t gz_raw;
    float gz_sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        // H:8bit shift, Link h and l
//        gz_raw = (int16_t)((uint16_t)(read_byte(GYRO_ZOUT_H) << 8) | (uint16_t)read_byte(GYRO_ZOUT_L));
        // printf("%d\r\n", gz_raw);
        gz = (float)(gz_raw / GYRO_FACTOR); // dps to rad/sec

        gz_sum += gz;
        HAL_Delay(1);
    }
    gz_offset = gz_sum / 1000.0f;
    // printf("%f\r\n", gyro_offset);
}

void UpdateGyroData()
{
    int16_t gz_raw;

    // H:8bit shift, Link h and l
//    gz_raw = (int16_t)((uint16_t)(read_byte(GYRO_ZOUT_H) << 8) | (uint16_t)read_byte(GYRO_ZOUT_L));
    gz = (float)(gz_raw / GYRO_FACTOR) - gz_offset; // dps to deg/sec

//    yaw += gz * CONTROL_PERIOD;
}
