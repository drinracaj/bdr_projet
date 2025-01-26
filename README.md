# Build and Run Instructions

## Windows

### Prerequisites
1. Install **Build Tools for Visual Studio 2022**:
    - Choose **Desktop development with C++**.
    - On the right panel, ensure the following are ticked:
        - **MSVC 2022**
        - **Windows 11 SDK**
2. Install **Qt6**:
    - Untick everything initially.
    - Under **Qt 6.8.1**:
        - Tick **Qt 6.8.1** and under it, **MSVC 2022 64-bit**.
    - Under **Build Tools**, tick **CMake 3.29.3**.
3. Install **PostgreSQL for Windows**.
4. Install **Docker** (ensure WSL is enabled).

### Environment Variable Configuration
Add the following paths to your `PATH` environment variable:
- `C:\Qt\Tools\CMake_64\bin`
- `C:\<Postgresql path>\PostgreSQL\<version>\bin`
- `C:\Qt\6.8.1\msvc2022_64\bin`

### Build Steps
1. Open **Developer Command Prompt for VS 2022**.
2. Navigate to the source directory:
   ```sh
   cd /path/to/src/dir
   ```
3. Create and navigate to a build directory:
   ```sh
   mkdir build
   cd build
   ```
4. Generate the build files:
   ```sh
   cmake ..
   ```
5. Build the project:
   ```sh
   cmake --build .
   ```

### Run the Project
1. From the project root directory, start the Docker containers:
   ```sh
   docker-compose up -d
   ```
2. Initialize the PostgreSQL database:
   ```sh
   docker exec -i bdr_postgresql_16 psql -U postgres -d nombd < init/01-init.sql
   ```
3. Launch the application:
   ```sh
   /build/Debug/rjmmo.exe
   ```

### Notes
- If you do not want to install all the dependencies, the variables to edit inside the `CMakeLists.txt` file can be found in the project's root directory.
- Using a compiler other than the one provided by Microsoft may cause issues, particularly with the PostgreSQL library.

## Linux / macOS

### Prerequisites
- **CMake**
- Build essentials for C/C++ (e.g., `make`, a C++20-compatible compiler, etc.)
- PostgreSQL (ensure `libpq` is installed)
- Docker and Docker Compose

### Build Steps
1. Navigate to the project's root directory and create a build directory:
   ```sh
   mkdir build && cd build
   ```
2. Generate the build files:
   ```sh
   cmake ..
   ```
3. Build the project:
   ```sh
   make
   ```

### Run the Project
1. Return to the project's root directory:
   ```sh
   cd ..
   ```
2. Start the Docker containers:
   ```sh
   docker-compose up -d
   ```
3. Initialize the PostgreSQL database:
   ```sh
   docker exec -i bdr_postgresql_16 psql -U postgres -d nombd < init/01-init.sql
   ```
4. Execute the application:
   ```sh
   ./build/rjmmo