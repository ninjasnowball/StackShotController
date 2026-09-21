// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

//Things for finding comm ports
#include <regex>
#include <filesystem>

#include <vector>
#include <algorithm>
#include <cctype>
#include <sys/ioctl.h>

unsigned char* readStack(int serial_port, ssize_t& num_bytes, int expected){
  // Allocate memory for read buffer
  static unsigned char read_buf[256];

  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes
  int bytes = 0;
  ioctl(serial_port, FIONREAD, &bytes);
  while(bytes < expected){
    printf(".");
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    ioctl(serial_port, FIONREAD, &bytes);
  }
  printf("\n");
  //printf("Number of bytes in buffer: %d \n", bytes);

  num_bytes = read(serial_port, read_buf, bytes);

  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  return read_buf;
}

bool waitStackMove(int serial_port, int axis){
    unsigned char axisCH = (unsigned char) axis;
    unsigned char axisAction = ((axisCH << 4 )|(0x00));
    unsigned char msgStatus[] = {0x55,0x10,0x05,axisAction,0x04,0x00,0x00,0x00,0x00,0x69};
    write(serial_port, msgStatus, sizeof(msgStatus));

    ssize_t num_bytes;
    unsigned char* response = readStack(serial_port, num_bytes, 10);

    /*
    printf("Rail Status: Read %zu bytes. Received message:", num_bytes);
    for (int i = 0; i <num_bytes; i++){
      printf("%02x ", response[i]);
    }
    printf("\n");
    */
    switch (response[5]){
        case 0:
            printf("Idle\n");
            break;
        case 1:
            printf("Moving\n");
            break;
        case 2:
            printf("Shutter\n");
            break;
        case 4:
            printf("Aborted\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    return response[5] != 0x00;
}

int stackMove(int serial_port, float rev, float speed,int dir, int axis){
  float steps = rev * 3200;
  const unsigned char * rv = reinterpret_cast<const unsigned char*>(&steps);
  const unsigned char * sp = reinterpret_cast<const unsigned char*>(&speed);
  const unsigned char  dirx = (unsigned char)dir;
  unsigned char axisCH = (unsigned char) axis;
    unsigned char axisAction = ((axisCH << 4 )|(0x01));

  unsigned char msgMove[] = {0x55,0x10,0x07,axisAction,0x0a,dirx,0x02,rv[0],rv[1],rv[2],rv[3],sp[0],sp[1],sp[2],sp[3],0x69};

  printf("Move message: ");
  for(int i = 0;i<sizeof(msgMove);i++){
    printf("%02x ", msgMove[i]);
  }
  printf("\n");

  write(serial_port, msgMove, sizeof(msgMove));

  ssize_t num_bytes;
  unsigned char* response = readStack(serial_port, num_bytes,6);

  // num_bytes is the number of bytes read. It may be 0 if no bytes were received, and can also be -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      close(serial_port);
      return -1;
  }

  printf("Read %zu bytes. Received move response:", num_bytes);
  for (int i = 0; i <num_bytes; i++){
      printf("%02x ", response[i]);
  }
  printf("\n");

  while(waitStackMove(serial_port,axis)) std::this_thread::sleep_for(std::chrono::milliseconds(5));

  return 0;
}

int stackMoveDiagonal(int serial_port, float x_rev, float x_speed, int x_dir, float y_rev, float y_speed, int y_dir){
    float x_steps = x_rev * 3200;
    float y_steps = y_rev * 3200;

    const unsigned char * x_rv = reinterpret_cast<const unsigned char*>(&x_steps);
    const unsigned char * x_sp = reinterpret_cast<const unsigned char*>(&x_speed);

    const unsigned char * y_rv = reinterpret_cast<const unsigned char*>(&y_steps);
    const unsigned char * y_sp = reinterpret_cast<const unsigned char*>(&y_speed);

    const unsigned char  x_dr = (unsigned char)x_dir;
    const unsigned char  y_dr = (unsigned char)y_dir;
    
    unsigned char y_axis = (unsigned char) 1;
    unsigned char x_axis = (unsigned char) 0;
    
    unsigned char x_action = ((x_axis << 4 )|(0x01));
    unsigned char y_action = ((y_axis << 4 )|(0x01));

    unsigned char x_move[] = {0x55,0x10,0x07,x_action,0x0a,x_dr,0x02,x_rv[0],x_rv[1],x_rv[2],x_rv[3],x_sp[0],x_sp[1],x_sp[2],x_sp[3],0x69};
    unsigned char y_move[] = {0x55,0x10,0x07,y_action,0x0a,y_dr,0x02,y_rv[0],y_rv[1],y_rv[2],y_rv[3],y_sp[0],y_sp[1],y_sp[2],y_sp[3],0x69};

    write(serial_port, x_move, sizeof(x_move));

    ssize_t x_num_bytes;
    unsigned char* x_response = readStack(serial_port, x_num_bytes,6);

    if (x_num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        close(serial_port);
        return -1;
    }

    printf("Read %zu bytes. Received move response:", x_num_bytes);
    for (int i = 0; i <x_num_bytes; i++){
        printf("%02x ", x_response[i]);
    }
    printf("\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    write(serial_port, y_move, sizeof(y_move));

    ssize_t y_num_bytes;
    unsigned char* y_response = readStack(serial_port, y_num_bytes,6);

    if (y_num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        close(serial_port);
        return -1;
    }

    printf("Read %zu bytes. Received move response:", y_num_bytes);
    for (int i = 0; i <y_num_bytes; i++){
        printf("%02x ", y_response[i]);
    }
    printf("\n");

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if(!waitStackMove(serial_port, 0) && !waitStackMove(serial_port, 1)) break;
    }

    return 0;

}

std::vector<std::string> get_list_serial_ports()
{
	std::vector<std::string> port_list;
	namespace fs = std::filesystem;
	const std::string DEV_PATH = "/dev";
	const std::regex base_regex(R"(\/dev\/cu..*)");
    try
    {
        fs::path p(DEV_PATH);
        if (!fs::exists(DEV_PATH)) return port_list;
        for (fs::directory_entry de: fs::directory_iterator(p)) {
            fs::path canonical_path = fs::canonical(de);
            std::string name = canonical_path.generic_string();
            std::smatch res;
            std::regex_search(name, res, base_regex);
            if (res.empty()) continue;
            port_list.push_back(canonical_path.generic_string());
        }
    }
    catch (const fs::filesystem_error &ex) {}
	std::sort(port_list.begin(), port_list.end());
	return port_list;
}

void stackClose(int serial_port){
  unsigned char msgClos[] = {0x55,0x11,0x0E,0x01,0x00,0x69};
  
  write(serial_port,msgClos,sizeof(msgClos));
  
  ssize_t num_bytes = 0;
  unsigned char* response = readStack(serial_port, num_bytes,6);

  printf("Read %zu bytes. Received closing response:", num_bytes);
  for (int i = 0; i <num_bytes; i++){
      printf("%02x ", response[i]);
  }

  printf("\n");

  close(serial_port);
}

int stackOpen(){
  std::cout << "Getting all available serial ports:" << std::endl;
  std::vector<std::string> ports = get_list_serial_ports();
  std::string serial;

  std::smatch match;
  for (const std::string port:ports){
    std::cout << port << std::endl;
    std::regex serial_regex(R"(/dev/cu\.usbserial-(.+))");
    if (std::regex_match(port, match, serial_regex)) {
        serial = match[1].str();
    }
  }

  std::string port_to_open = "/dev/cu.usbserial-" + serial;
  const char* port_char = port_to_open.c_str();

  int serial_port = open(port_char, O_RDWR);
  std::cout << "Opened port: "<< serial_port <<std::endl;
  // Check for errors

  if (serial_port < 0) {
      printf("Error %i from open: %s\n", errno, strerror(errno));
      return -1;
  }

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if (tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      close(serial_port);
      return -1;
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

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B38400);
  cfsetospeed(&tty, B38400);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      close(serial_port);
      return -1;
  }

  return serial_port;
}
