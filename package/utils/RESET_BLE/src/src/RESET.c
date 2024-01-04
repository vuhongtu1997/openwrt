/* standard headers */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sqlite3.h>

// transmit
static char getpro[]                  =   {0xe9, 0xff, 0x0c};
static char reset_gateway[]           =   {0xe9, 0xff, 0x02};
static char setpro_internal[]       =   {0xe9, 0xff, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33, 0x44, 0x01, 0x00};
static char admit_pro_internal[]    =   {0xe9, 0xff, 0x0d, 0x01, 0x00, 0xff, 0xfb, 0xeb, 0xbf, 0xea, 0x06, 0x09, 0x00, 0x52, 0x90, 0x49, 0xf1, 0xf1, 0xbb, 0xe9, 0xeb};// trả về unicast tiếp theo của con đèn cần thêm vào

int serial_port;

int UART_Init()
{
	serial_port = open("/dev/ttyS1", O_RDWR);
	struct termios tty;
	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) {
	  printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	  return 1;
	}

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	cfsetispeed(&tty, B115200);
	cfsetospeed(&tty, B115200);

	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return 1;
	}
}

void RESET_GW()
{
	write(serial_port, reset_gateway, sizeof(reset_gateway));
}

void openfile(const char* command)
{
	FILE *file;
	char msg_rsp[2048]={0};
	char msg_line[100]={0};
	file = popen(command, "r");
	if(file==NULL)
	{
		puts("lỗi");
		exit(1);
	}
	fgets(msg_line, 100, file);
	strcat(msg_rsp,msg_line);
	while(1)
	{
		fgets(msg_line, 100, file);
		if(feof(file))
		{
			break;
		}
		strcat(msg_rsp,msg_line);
	}
	pclose(file);
}

int DBSQL(char* sql) {
	sqlite3* DB;
	int exit = 0;
	do {
		exit = sqlite3_open("/root/rd.Sqlite", &DB);
	} while (exit != SQLITE_OK);
	char* messaggeError = 0;
	while(sqlite3_exec(DB, sql, NULL, 0, &messaggeError) != SQLITE_OK){
		sqlite3_free(messaggeError);
	}
	if (exit != SQLITE_OK) {
		sqlite3_free(messaggeError);
	}
	else
		sqlite3_close(DB);
	return (0);
}

void ResetDB(){
	DBSQL("DELETE FROM Device;");
	DBSQL("DELETE FROM DeviceAttributeValue;");
	DBSQL("DELETE FROM EventTrigger;");
	DBSQL("DELETE FROM EventTriggerID;");
	DBSQL("DELETE FROM EventTriggerInputDeviceMapping;");
	DBSQL("DELETE FROM EventTriggerInputGroupingMapping;");
	DBSQL("DELETE FROM EventTriggerOutputDeviceMapping;");
	DBSQL("DELETE FROM EventTriggerOutputDeviceSetupValue;");
	DBSQL("DELETE FROM EventTriggerOutputGroupingMapping;");
	DBSQL("DELETE FROM EventTriggerOutputGroupingSetupValue;");
	DBSQL("DELETE FROM EventTriggerOutputSceneMapping;");
	DBSQL("DELETE FROM GROUPID;");
	DBSQL("DELETE FROM GROUPING;");
	DBSQL("DELETE FROM QueueMsgRsp;");
	DBSQL("DELETE FROM GroupingDeviceMapping;");
	DBSQL("DELETE FROM SystemConfiguration;");
	DBSQL("DELETE FROM UserData;");

	openfile("uci set network.wan.ifname='eth0'");
	openfile("uci commit network");
	openfile("/etc/init.d/network restart");
	openfile("uci del wireless.wifinet1");
	openfile("uci commit wireless");
	openfile("wifi");
	openfile("rm /root/device_key.txt");
	openfile("rm /etc/Dormitory.txt");
	sleep(15);
	openfile("reboot -f");
}

int main(int argc, char** argv)
{
	UART_Init();
	puts("INIT UART DONE");
	RESET_GW();
	puts("RESET GW DONE");
	sleep(6);
	ResetDB();
	puts("DONE!");
}
