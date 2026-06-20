# Habit Tracker

A simple, modular C++ CLI application for tracking habits and streaks.

## Project Overview

The **Habit Tracker** is designed to help users build and maintain habits by tracking daily completion (check-ins) and calculating streaks. It follows a classic 3-tier architecture to ensure separation of concerns and maintainability.

### Architecture

- **Data Layer (`Habit`, `FileManager`)**: Handles habit entities and their persistence to a CSV file (`habits.csv`).
- **Controller/Logic Layer (`HabitManager`)**: Coordinates business logic, such as adding/removing habits, calculating IDs, and performing check-ins.
- **UI Layer (`main.cpp`)**: Responsible for interacting with the user via a terminal menu, handling input, and displaying the current date.

### Key Features

- **Habit Management**: Add, remove, and list habits.
- **Streak Calculation**: Automatically updates current and best streaks based on check-in dates.
- **Persistence**: Saves data to a local `habits.csv` file, allowing data to persist between sessions.
- **Robustness**: Includes basic date manipulation logic to handle streak continuity (e.g., checking if the last check-in was "yesterday").

## Building and Running

### Prerequisites

- A C++ compiler (e.g., `g++`, `clang++`, or MSVC).
- `make` or `mingw32-make` (optional, for using the Makefile).
- Standard C++ library.

### Compilation

You can use the provided `Makefile` to build the project:

```bash
# Using mingw32-make (Windows)
mingw32-make

# Or using standard make (Linux/macOS)
make
```

Alternatively, compile manually:

```bash
g++ -o habit_tracker *.cpp
```

### Running

After compilation, run the executable:

```bash
# Windows
.\habit_tracker.exe

# Linux/macOS
./habit_tracker
```

## Development Conventions

- **Naming**: Uses `camelCase` for variables and methods, and `PascalCase` for class names.
- **Documentation**: Code is documented using Doxygen-style comments (`/** ... */`) in header files.
- **Standard Library**: Extensively uses `std::vector`, `std::string`, and `<fstream>` for data management and I/O.
- **Error Handling**: Uses basic `try-catch` blocks for CSV parsing and `std::cerr` for reporting I/O errors.
- **Separation of Concerns**: Each class has a single responsibility (SRP). Business logic should stay in `HabitManager` or `Habit`, while I/O belongs to `FileManager`.

## File Structure

- `main.cpp`: The entry point of the application, providing the CLI menu.
- `Habit.h/cpp`: The core data entity representing a single habit.
- `HabitManager.h/cpp`: The business logic controller for managing multiple habits.
- `FileManager.h/cpp`: Handles reading and writing habits to a CSV file.
- `Makefile`: Build script for the project.
- `habits.csv`: The default data storage file (created on first run).
