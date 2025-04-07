# Smart Attendance System

A web-based attendance system that allows students to mark their attendance and teachers to view and download attendance records.

## Features

- Student attendance marking with location verification
- Teacher dashboard to view attendance records
- Download attendance records in PDF or TXT format
- CSV-based student database
- Simple and intuitive user interface

## Deployment on Render.com

1. Fork this repository to your GitHub account
2. Sign up for a free account on [Render.com](https://render.com)
3. Click "New +" and select "Web Service"
4. Connect your GitHub repository
5. Configure the service:
   - Name: smart-attendance-system
   - Environment: Docker
   - Branch: main
   - Build Command: `docker build -t smart-attendance .`
   - Start Command: `docker run -p $PORT:8080 smart-attendance`
6. Click "Create Web Service"

The service will be deployed and you'll get a URL to access your application.

## Local Development

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/smart-attendance-system.git
   cd smart-attendance-system
   ```

2. Compile the server:
   ```bash
   gcc server.c -o server
   ```

3. Run the server:
   ```bash
   ./server
   ```

4. Access the application at `http://localhost:8080`

## File Structure

- `server.c` - Main server application
- `index.html` - Student attendance page
- `teacher_dashboard.html` - Teacher dashboard
- `test.html` - Test environment without location verification
- `students.csv` - Student database
- `styles.css` - CSS styles
- `script.js` - JavaScript functions

## Security Considerations

- The system uses location verification to ensure students are within the college premises
- Teacher dashboard is password protected
- All data is stored locally and can be downloaded for backup

## License

This project is licensed under the MIT License - see the LICENSE file for details.

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
