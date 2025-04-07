# Deployment Guide for Attendance System on Render.com

This guide will walk you through deploying your attendance system on Render.com, making it accessible from anywhere on the internet.

## Prerequisites

1. A GitHub account
2. A Render.com account (free tier is sufficient)
3. Your attendance system code (server.c, HTML files, etc.)

## Step 1: Prepare Your Code for Deployment

### 1.1. Create a GitHub Repository

1. Go to [GitHub](https://github.com) and sign in
2. Click the "+" icon in the top right corner and select "New repository"
3. Name your repository (e.g., "attendance-system")
4. Make it public or private as per your preference
5. Click "Create repository"

### 1.2. Modify Your Server Code

You need to modify your server.c file to work with Render's environment:

```c
// Add at the top of server.c
#include <stdlib.h>

// In the main() function, replace the hardcoded port with:
char* port_str = getenv("PORT");
int port = port_str ? atoi(port_str) : 8080;

// Use 'port' variable instead of hardcoded 8080 in the bind() function
```

### 1.3. Create a Dockerfile

Create a file named `Dockerfile` (no extension) in your project root:

```dockerfile
FROM gcc:latest

WORKDIR /app
COPY . .

RUN gcc server.c -o app -lwsock32 -lws2_32

EXPOSE 8080

CMD ["./app"]
```

### 1.4. Create render.yaml

Create a file named `render.yaml` in your project root:

```yaml
services:
  - type: web
    name: attendance-system
    env: docker
    buildCommand: gcc server.c -o app -lwsock32 -lws2_32
    startCommand: ./app
    envVars:
      - key: PORT
        value: 8080
```

### 1.5. Update HTML Files

Update all your HTML files to use relative URLs instead of hardcoded IP addresses:

- Replace `http://192.168.79.128:8080` with relative paths like `/verify_student` or `/mark_attendance`

### 1.6. Create a .gitignore File

Create a file named `.gitignore`:

```
*.exe
*.o
*.obj
.DS_Store
```

## Step 2: Push Your Code to GitHub

1. Open a terminal/command prompt
2. Navigate to your project directory
3. Run the following commands:

```bash
git init
git add .
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
git push -u origin main
```

Replace `YOUR_USERNAME` and `YOUR_REPO_NAME` with your actual GitHub username and repository name.

## Step 3: Deploy on Render.com

### 3.1. Create a Render Account

1. Go to [Render.com](https://render.com) and sign up for a free account
2. Verify your email address

### 3.2. Create a New Web Service

1. Log in to your Render dashboard
2. Click "New +" in the top right corner
3. Select "Web Service" from the dropdown menu

### 3.3. Connect Your GitHub Repository

1. Select "GitHub" as your repository source
2. Authorize Render to access your GitHub account if prompted
3. Select your attendance system repository from the list

### 3.4. Configure Your Web Service

1. **Name**: Enter a name for your service (e.g., "attendance-system")
2. **Environment**: Select "Docker"
3. **Branch**: Select "main" (or your default branch)
4. **Region**: Choose the region closest to your users
5. **Instance Type**: Select "Free" for testing (you can upgrade later)
6. **Build Command**: Leave as is (will use the one in render.yaml)
7. **Start Command**: Leave as is (will use the one in render.yaml)

### 3.5. Deploy Your Service

1. Click "Create Web Service"
2. Render will start building and deploying your application
3. This process may take a few minutes

## Step 4: Access Your Deployed Application

1. Once deployment is complete, Render will provide you with a URL (e.g., `https://attendance-system.onrender.com`)
2. Click on the URL to access your deployed application
3. Share this URL with your students and teachers

## Step 5: Update Your Application (When Needed)

Whenever you make changes to your code:

1. Commit and push your changes to GitHub:
   ```bash
   git add .
   git commit -m "Description of changes"
   git push
   ```

2. Render will automatically detect the changes and redeploy your application

## Troubleshooting

### Common Issues and Solutions

1. **Build Failures**:
   - Check the build logs in Render dashboard
   - Ensure your Dockerfile and render.yaml are correctly formatted
   - Verify that all required files are included in your repository

2. **Application Not Starting**:
   - Check the logs in Render dashboard
   - Verify that your server is binding to the correct port
   - Ensure your application is properly handling the PORT environment variable

3. **Connection Issues**:
   - Verify that your HTML files are using the correct URLs
   - Check if your server is properly handling CORS requests

### Getting Help

- Render provides detailed logs for debugging
- You can contact Render support for assistance
- Check the [Render documentation](https://render.com/docs) for more information

## Security Considerations

1. **HTTPS**: Render automatically provides HTTPS for your application
2. **Environment Variables**: Use Render's environment variables for sensitive information
3. **Rate Limiting**: Consider implementing rate limiting to prevent abuse
4. **Data Backup**: Regularly backup your students.csv file

## Maintenance

1. **Monitoring**: Check your Render dashboard regularly for any issues
2. **Updates**: Keep your code updated with security patches
3. **Scaling**: If your application grows, consider upgrading your Render plan

## Additional Resources

- [Render Documentation](https://render.com/docs)
- [GitHub Documentation](https://docs.github.com)
- [Docker Documentation](https://docs.docker.com)

---

This guide should help you successfully deploy your attendance system on Render.com. If you encounter any issues, refer to the troubleshooting section or contact Render support. 