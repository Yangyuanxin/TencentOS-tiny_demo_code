#include "demo.h"

#define WIFI_SSID		"602"
#define WIFI_PASSWD		"13602584536"
#define HTTP_DATA_MAX_LEN 50
#define HTTP_HEAD_MAX_LEN 130
#define HTTP_SRC_DATA_LEN 200
//服务器地址
#define SERVER_IP     "192.168.4.1"
//服务器端口号
#define SERVER_PORT   8080

/*连接标志*/
__IO uint8_t Connect_Flag = 0 ;


extern k_event_t key_event;
extern const k_event_flag_t event_key1 ;
extern const k_event_flag_t event_key2 ;
extern const k_event_flag_t event_key3 ;
extern const k_event_flag_t event_key4 ;
extern const k_event_flag_t event_key5 ;


void tcp_test(void)
{
		k_event_flag_t flag_match;
    k_err_t err;
    int recv_len = -1;
    int fd, rc, cnt = 0;

    struct sockaddr_in addr;
		
		
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr.sin_port = htons(SERVER_PORT);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("socket failed\n");
        return;
    }

    rc = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0) {
        printf("connect failed\n");
        close(fd);
        return;
    }
		Connect_Flag = 1 ;
		while(1)
		{
				if(Connect_Flag)
				{
					//同时监视4个事件
					err = tos_event_pend(&key_event, event_key5 | event_key1 | event_key2 | event_key3 | event_key4,
															 &flag_match, TOS_TIME_FOREVER, TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR); //

					if (err == K_ERR_NONE)
					{
							if (flag_match == event_key1)
							{
									printf("按键1\n");
									HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
									send(fd, (const void*)"GO", strlen("GO"), 0);
							}

							if (flag_match == event_key2)
							{
									printf("按键2\n");
									HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
									send(fd, (const void*)"BACK", strlen("BACK"), 0);
							}

							if (flag_match == event_key3)
							{
									printf("按键3\n");
									HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
									send(fd, (const void*)"LEFT", strlen("LEFT"), 0);
							}

							if (flag_match == event_key4)
							{
									printf("按键4\n");
									HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
									send(fd, (const void*)"RIGHT", strlen("RIGHT"), 0);
							}

							if (flag_match == event_key5)
							{
									printf("按键状态清除\n");
									HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
									send(fd, (const void*)"STOP", strlen("STOP"), 0);
							}
					}
				}
		}
    close(fd);
}


#define TCP_TEST_TASK_STK_SIZE         4096
void tcp_test(void);
osThreadDef(tcp_test, osPriorityNormal, 1, TCP_TEST_TASK_STK_SIZE);

void application_entry_demo1(void *arg)
{
	int ret = 0 ;
	ret = esp8266_sal_init(HAL_UART_PORT_0);
	if(ret != 0 )
	{
		printf("esp8266 sal初始化失败\n");
		return ;
	}
	printf("esp8266 sal初始化成功\n");
    esp8266_join_ap(WIFI_SSID, WIFI_PASSWD);
	osThreadCreate(osThread(tcp_test), NULL);
}

