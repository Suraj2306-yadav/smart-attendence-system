# Smart Attendance System

A modern attendance management system that uses QR codes and location verification to ensure students are physically present when marking attendance.

## Features

- **QR Code Based Attendance**: Teachers generate QR codes that students scan to mark attendance
- **Location Verification**: Ensures students are physically present at the college
- **Teacher Dashboard**: Monitor attendance records and download reports
- **Mobile Friendly**: Works on any device with a camera and location services
- **Password Protected**: Teacher dashboard is secured with password

## Setup Instructions

### Prerequisites

- GCC compiler (for Windows, use MinGW)
- Web browser with camera access
- Location services enabled on mobile devices

### Installation

1. Clone the repository:
   ```
   git clone https://github.com/Suraj2306-yadav/smart-attendence-system.git
   cd smart-attendence-system
   ```

2. Compile the server:
   ```
   gcc server.c -o server.exe -lwsock32 -lws2_32
   ```

3. Run the server:
   ```
   ./server.exe
   ```

4. Access the application:
   - Teacher Dashboard: http://localhost:8080/teacher_dashboard.html
   - Student Page: http://localhost:8080/student.html
   - Test Page: http://localhost:8080/test.html

## Usage

### For Teachers

1. Open the teacher dashboard
2. Enter password: smart@123
3. Generate a QR code
4. Monitor attendance records
5. Download attendance reports as PDF or TXT

### For Students

1. Open the student page on your mobile device
2. Allow location access when prompted
3. Scan the QR code displayed by the teacher
4. Attendance will be marked automatically if you're at the college location

## Security Features

- Password protected teacher dashboard
- QR codes expire after 5 minutes
- Location verification prevents proxy attendance
- Secure data handling

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Files

- `server.c` - Backend server handling attendance logic
- `teacher_dashboard.html` - Teacher's interface for generating QR codes and viewing attendance
- `student_attendance.html` - Student interface for marking attendance
- `students.csv` - Database of student information

## College Location

Avviare Educational Hub
A/20, C-56, near Fortis Metro Station
C Block, Phase 2, Industrial Area
Sector 62, Noida, Uttar Pradesh 201301

## Notes

- The system verifies student location within 100 meters of the college coordinates
- Students must enable location services on their devices
- Internet connection is required for both teachers and students
- The system uses local storage for demonstration purposes 