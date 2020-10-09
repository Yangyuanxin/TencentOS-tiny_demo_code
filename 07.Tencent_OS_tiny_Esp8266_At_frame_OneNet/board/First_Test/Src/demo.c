#include "demo.h"

#define WIFI_SSID 				"SRED"
#define WIFI_PASSWORD			"12345678"
#define SERVER_ADDR				"183.230.40.33"
#define SERVER_PORT				80
#define ONENET_DEVICE_ID  		"604618155"
#define ONENET_API_KEY	  		"PShVObl=1Sx87Or4lI3VOPGEpaY="

#define http_uploader_head	\
    "POST /devices/%s/datapoints?type=3 HTTP/1.1\r\n"	\
    "api-key:%s\r\n"					\
    "Host:api.heclouds.com\r\n"									\
    "Content-Length:%d\r\n\r\n"

#define http_uploader_data \
    "{\"lux\":%d}"
	
char buf[512] = {0};
char buf_data[120] = {0};
void OneNet_Packet_Send(int fd,const char *device_id, const char *api_key, int lux)
{
    int len = 0 ;
    
    sprintf(buf_data, http_uploader_data, lux);
    len = strlen(buf_data);
    sprintf(buf, http_uploader_head, device_id, api_key, len);
    strcat(buf, buf_data);
    printf("数据包:\n");
    printf("%s", buf);
	send(fd,buf,strlen(buf),0);
}

void OneNet_SendData(int fd)
{
    OneNet_Packet_Send(fd,ONENET_DEVICE_ID, ONENET_API_KEY, rand() % 50000);
}	

#define RECV_LEN            1024
uint8_t recv_data[RECV_LEN];

void tcp_test(void)
{
    int recv_len = -1;
    int fd, rc, cnt = 0;

    struct sockaddr_in addr;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
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

    while (1) {
      
		OneNet_SendData(fd);
        recv_len = recv(fd, recv_data, sizeof(recv_data), 0);
        if (recv_len < 0) {
            printf("task receive error\n");
        } else if (recv_len == 0) {
            printf("task receive none\n");
        } else {
            recv_data[recv_len] = 0;
            printf("task: receive len: %d\nmsg from remote: %s\n", recv_len, recv_data);
        }
        if (++cnt >= 20) {
            break;
        }
        tos_sleep_ms(5000);
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
    esp8266_join_ap(WIFI_SSID, WIFI_PASSWORD);
	osThreadCreate(osThread(tcp_test), NULL);
}

