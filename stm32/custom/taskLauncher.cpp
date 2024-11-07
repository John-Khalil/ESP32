// #pragma once

// #include "platformUtils.cpp"
// #include "headlessCommunication.cpp"

// #include <stdint.h>
// #include <functional>
// #include <vector>
// #include <memory>
// #include <algorithm>
// #include <string>
// #include <type_traits>

// #include <STM32FreeRTOS.h>

// void task1(void *pvParameters){
//     // uint64_t test[2]={};
//     // uint32_t loopCounter=0;
//     // while(1){
//     //     vTaskDelay(10);
//     //     test[(loopCounter++)%2]=GET_TIMER();
//     //     HW_DELAY(100000);
//     //     test[(loopCounter++)%2]=GET_TIMER();
//     //     Serial.print(GET_TIMER());
//     //     headlessAttribute(" -> task1_",(test[0]>test[1])?(test[0]-test[1]):(test[1]-test[0]));
//     //     // Serial.print("task1 >>> ");
//     //     // Serial.println((test[0]>test[1])?(test[0]-test[1]):(test[1]-test[0]));
//     // }
//     // return;
//     while(1)
//         vTaskDelay(100000);
// }

// void task2(void *pvParameters){
//     uint64_t test[2]={};
//     uint32_t loopCounter=0;
//     while(1){
//         vTaskDelay(1);
//         test[(loopCounter++)%2]=GET_TIMER();
//         vTaskDelay(5);
//         test[(loopCounter++)%2]=GET_TIMER();
//         headlessAttribute("task2_",(test[0]>test[1])?(test[0]-test[1]):(test[1]-test[0]));
//         // Serial.print("task2 >>> ");
//         // Serial.println((test[0]>test[1])?(test[0]-test[1]):(test[1]-test[0]));
//     }
//     return;
// }

// void headlessEndpointTask(void *pvParameters){
//     headlessEndpoint();
//     return;
// }

// void taskLauncher(void){
//     xTaskCreate(
//         task1,
//         (const portCHAR *) "",
//         128,  // Stack size
//         NULL,
//         1,  // Priority
//         NULL
//     );
//     // xTaskCreate(
//     //     task2,
//     //     (const portCHAR *) "",
//     //     128,  // Stack size
//     //     NULL,
//     //     1,  // Priority
//     //     NULL
//     // );

//     xTaskCreate(
//         headlessEndpointTask,
//         (const portCHAR *) "",
//         256,  // Stack size
//         NULL,
//         1,  // Priority
//         NULL
//     );

    
//     vTaskStartScheduler();
//     return;
// }