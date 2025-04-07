#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")

// Use PORT environment variable with fallback to 8080
#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_STUDENTS 100
#define MAX_ATTENDANCE 1000

typedef struct {
    char enrollment_no[20];
    char name[50];
    char father_name[50];
} Student;

typedef struct {
    char enrollment_no[20];
    char timestamp[30];
} Attendance;

Student students[MAX_STUDENTS];
int student_count = 0;
Attendance attendance_records[MAX_ATTENDANCE];
int attendance_count = 0;

// College coordinates (Avviare Educational Hub)
const double COLLEGE_LAT = 28.6266;
const double COLLEGE_LNG = 77.3666;

// Function to get content type based on file extension
const char* get_content_type(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (!dot) return "text/plain";
    if (strcmp(dot, ".html") == 0) return "text/html";
    if (strcmp(dot, ".css") == 0) return "text/css";
    if (strcmp(dot, ".js") == 0) return "text/javascript";
    if (strcmp(dot, ".json") == 0) return "application/json";
    return "text/plain";
}

void load_students() {
    FILE *file = fopen("students.csv", "r");
    if (file == NULL) {
        printf("Error opening students.csv\n");
        return;
    }

    char line[256];
    // Skip header
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) && student_count < MAX_STUDENTS) {
        char *token = strtok(line, ",");
        strcpy(students[student_count].enrollment_no, token);
        
        token = strtok(NULL, ",");
        strcpy(students[student_count].name, token);
        
        token = strtok(NULL, ",\n");
        strcpy(students[student_count].father_name, token);
        
        student_count++;
    }
    fclose(file);
}

// Function to send a file
void send_file(SOCKET client_socket, const char* filename) {
    printf("Attempting to serve file: %s\n", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", filename);
        char response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nFile not found";
        send(client_socket, response, strlen(response), 0);
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Prepare and send headers
    char headers[512];
    sprintf(headers, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n",
        get_content_type(filename),
        file_size
    );
    send(client_socket, headers, strlen(headers), 0);

    // Send file content
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
    printf("File served successfully: %s\n", filename);
}

Student* find_student(const char* enrollment_no) {
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].enrollment_no, enrollment_no) == 0) {
            return &students[i];
        }
    }
    return NULL;
}

void mark_attendance(const char* enrollment_no) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    strcpy(attendance_records[attendance_count].enrollment_no, enrollment_no);
    strftime(attendance_records[attendance_count].timestamp, 
            sizeof(attendance_records[attendance_count].timestamp),
            "%Y-%m-%d %H:%M:%S", t);
    
    attendance_count++;
}

// Function to extract file path without query parameters
void extract_file_path(const char* full_path, char* file_path, int max_length) {
    // Copy up to the first '?' or the end of the string
    int i;
    for (i = 0; i < max_length - 1 && full_path[i] != '\0' && full_path[i] != '?'; i++) {
        file_path[i] = full_path[i];
    }
    file_path[i] = '\0';
}

void handle_request(SOCKET client_socket, const char* request) {
    char response[BUFFER_SIZE];
    char* file_path = extract_file_path(request);
    
    if (!file_path) {
        sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // Handle attendance submission
    if (strstr(request, "/mark_attendance")) {
        // Extract enrollment number from query parameters
        char* query = strstr(request, "?d=");
        if (query) {
            query += 3; // Skip "?d="
            
            // Parse JSON data
            char enrollment[20] = {0};
            char name[100] = {0};
            char father_name[100] = {0};
            
            // Extract enrollment number from JSON
            char* enroll_start = strstr(query, "\"l\":\"");
            if (enroll_start) {
                enroll_start += 5;
                char* enroll_end = strchr(enroll_start, '\"');
                if (enroll_end) {
                    strncpy(enrollment, enroll_start, enroll_end - enroll_start);
                    enrollment[enroll_end - enroll_start] = '\0';
                }
            }
            
            // Find student in students.csv
            FILE* student_file = fopen("students.csv", "r");
            if (student_file) {
                char line[256];
                while (fgets(line, sizeof(line), student_file)) {
                    char file_enrollment[20];
                    sscanf(line, "%[^,],%[^,],%[^\n]", file_enrollment, name, father_name);
                    if (strcmp(file_enrollment, enrollment) == 0) {
                        break;
                    }
                }
                fclose(student_file);
            }
            
            // Add to attendance records
            if (attendance_count < MAX_ATTENDANCE) {
                strcpy(attendance_records[attendance_count].enrollment_no, enrollment);
                strcpy(attendance_records[attendance_count].name, name);
                strcpy(attendance_records[attendance_count].father_name, father_name);
                get_current_timestamp(attendance_records[attendance_count].timestamp, sizeof(attendance_records[attendance_count].timestamp));
                attendance_count++;
                
                sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 2\r\n\r\n{}");
            } else {
                sprintf(response, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
            }
        } else {
            sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        }
    }
    // Handle attendance data request
    else if (strcmp(file_path, "/get_attendance") == 0) {
        char json_data[BUFFER_SIZE * 10] = "[";
        for (int i = 0; i < attendance_count; i++) {
            char record[256];
            sprintf(record, "{\"enrollment_no\":\"%s\",\"name\":\"%s\",\"father_name\":\"%s\",\"timestamp\":\"%s\"}",
                    attendance_records[i].enrollment_no,
                    attendance_records[i].name,
                    attendance_records[i].father_name,
                    attendance_records[i].timestamp);
            
            strcat(json_data, record);
            if (i < attendance_count - 1) {
                strcat(json_data, ",");
            }
        }
        strcat(json_data, "]");
        
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
                strlen(json_data), json_data);
    }
    // Serve HTML, CSS, and JS files
    else {
        // Default to index.html if root path
        if (strcmp(file_path, "/") == 0) {
            strcpy(file_path, "/index.html");
        }
        
        // Remove leading slash
        if (file_path[0] == '/') {
            file_path++;
        }
        
        // Open and read the requested file
        FILE* file = fopen(file_path, "rb");
        if (file) {
            // Get file size
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);
            
            // Determine content type
            const char* content_type = "text/plain";
            if (strstr(file_path, ".html")) content_type = "text/html";
            else if (strstr(file_path, ".css")) content_type = "text/css";
            else if (strstr(file_path, ".js")) content_type = "application/javascript";
            
            // Send HTTP header
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",
                    content_type, file_size);
            send(client_socket, response, strlen(response), 0);
            
            // Send file content
            char buffer[BUFFER_SIZE];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
                send(client_socket, buffer, bytes_read, 0);
            }
            
            fclose(file);
        } else {
            sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
            send(client_socket, response, strlen(response), 0);
        }
    }
    
    free(file_path);
}

int main() {
    // Get port from environment variable
    char* port_str = getenv("PORT");
    int port = port_str ? atoi(port_str) : DEFAULT_PORT;
    
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Load students from CSV
    load_students();
    
    // Create socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        printf("setsockopt failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    // Bind socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(server_fd, 10) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    printf("Server listening on port %d\n", port);
    
    // Accept connections
    while (1) {
        SOCKET client_socket;
        int addrlen = sizeof(struct sockaddr_in);
        struct sockaddr_in client_addr;
        
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }
        
        // Handle request
        char buffer[BUFFER_SIZE] = {0};
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            handle_request(client_socket, buffer);
        }
        
        closesocket(client_socket);
    }
    
    closesocket(server_fd);
    WSACleanup();
    return 0;
} 