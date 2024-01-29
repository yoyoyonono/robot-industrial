/**
 * RP2040 FreeRTOS Template
 * 
 * @copyright 2023, Tony Smith (@smittytone)
 * @version   1.4.2
 * @licence   MIT
 *
 */
#include "main.hpp"


/*
 * CONSTANTS
 */

#define RPLIDAR_MOTOR_PIN  20

/*
 * GLOBALS
 */
// This is the inter-task queue
volatile QueueHandle_t queue = NULL;

// Set a delay time of exactly 500ms
const TickType_t ms_delay = 500 / portTICK_PERIOD_MS;

// FROM 1.0.1 Record references to the tasks
TaskHandle_t gpio_task_handle = NULL;
TaskHandle_t pico_task_handle = NULL;


/*
 * FUNCTIONS
 */

/**
 * @brief Task to blink the PICO W LED.
 * 
 */

void task_blink_led(void* unused_arg) {
    if (!cyw43_arch_init()) {
        log_debug("CYW43 init failed");
        vTaskDelete(NULL);
    }
    log_debug("CYW43 init success");
    while(true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(ms_delay);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(ms_delay);    
    }
}

/**
 * @brief Task to read data from lidar
 * 
 */

void task_read_lidar(void* unused_arg) {
    gpio_put(PWM_MOTOR_PIN, 1);
    // everything else
}

/**
 * @brief Generate and print a debug message from a supplied string.
 *
 * @param msg: The base message to which `[DEBUG]` will be prefixed.
 */
void log_debug(const char* msg) {

#ifdef DEBUG
    uint msg_length = 9 + strlen(msg);
    char* sprintf_buffer = static_cast<char*>(malloc(msg_length));
    sprintf(sprintf_buffer, "[DEBUG] %s\n", msg);
    printf("%s", sprintf_buffer);
    free(sprintf_buffer);
#endif
}


/**
 * @brief Show basic device info.
 */
void log_device_info(void) {

    printf("App: %s %s (%i)\n", APP_NAME, APP_VERSION, BUILD_NUM);
}


/*
 * RUNTIME START
 */
int main() {

    // Enable STDIO
#ifdef DEBUG
    stdio_usb_init();
    // Pause to allow the USB path to initialize
    sleep_ms(2000);

    // Log app info
    log_device_info();
#endif

    // Set up the event queue
    queue = xQueueCreate(4, sizeof(uint8_t));

    vTaskStartScheduler();
    
    // We should never get here, but just in case...
    while(true) {
        tight_loop_contents();
    };
}