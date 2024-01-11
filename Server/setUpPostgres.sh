#!/bin/bash

# Hardcoded database credentials
DB_NAME="myappdb"
DB_USER="myappuser"
DB_PASS="myapppassword"

# PostgreSQL data directory within your project
PG_DATA_DIR="$HOME/CLionProjects/BongoSync/Server/postgres_data"

# Function to install and start PostgreSQL on macOS
install_postgres_mac() {
    echo "Installing PostgreSQL on macOS..."
    brew install postgresql
}

# Function to install and start PostgreSQL on Debian-based Linux
install_postgres_linux() {
    echo "Installing PostgreSQL on Linux..."
    sudo apt-get update
    sudo apt-get install postgresql postgresql-contrib
}

# Function to create a new PostgreSQL user and database
create_user_and_db() {
    # Determine the superuser to use
    if [[ "$OS" == "Darwin" ]]; then
        SUPERUSER=$(whoami)
    else
        SUPERUSER="postgres"
    fi

    echo "Creating new PostgreSQL user: $DB_USER"
    sudo -u $SUPERUSER psql -c "CREATE USER $DB_USER WITH PASSWORD '$DB_PASS';"
    echo "Creating new database: $DB_NAME"
    sudo -u $SUPERUSER createdb -O $DB_USER $DB_NAME
}

# Detecting the Operating System
OS="$(uname)"
echo "Operating System detected: $OS"

# Install PostgreSQL if not already installed
case "$OS" in
    "Darwin")
        if brew ls --versions postgresql > /dev/null; then
            echo "PostgreSQL is already installed."
        else
            install_postgres_mac
        fi
        ;;
    "Linux")
        if dpkg -l | grep -qw postgresql; then
            echo "PostgreSQL is already installed."
        else
            install_postgres_linux
        fi
        ;;
    *)
        echo "Unsupported operating system."
        exit 1
        ;;
esac

# Stop any running PostgreSQL service
echo "Stopping any running PostgreSQL service..."
brew services stop postgresql || sudo systemctl stop postgresql

export PATH="/opt/homebrew/opt/postgresql@14/bin:$PATH"

# Create and initialize the new PostgreSQL data directory
echo "Creating and initializing new PostgreSQL data directory at $PG_DATA_DIR..."
if [ -d "$PG_DATA_DIR" ]; then
    echo "Data directory exists. Clearing it..."
    rm -rf "$PG_DATA_DIR"/*
else
    mkdir -p "$PG_DATA_DIR"
fi
/opt/homebrew/opt/postgresql@14/bin/initdb "$PG_DATA_DIR"


# Configure PostgreSQL to use the new data directory
PG_SERVICE_FILE="/opt/homebrew/opt/postgresql@14/homebrew.mxcl.postgresql.plist"
if [ -f "$PG_SERVICE_FILE" ]; then
    cp "$PG_SERVICE_FILE" "$PG_SERVICE_FILE.bak"
    sed -i '' "s|/opt/homebrew/var/postgres|$PG_DATA_DIR|g" "$PG_SERVICE_FILE"
else
    echo "PostgreSQL service file not found. Skipping configuration."
fi


# Start PostgreSQL with the new configuration
echo "Starting PostgreSQL with the new configuration..."
brew services start postgresql || sudo systemctl start postgresql

# Get the current macOS username
CURRENT_USER=$(whoami)

# Wait for PostgreSQL to start up
echo "Waiting for PostgreSQL to start..."
until pg_isready -h localhost -d postgres -U $CURRENT_USER
do
  echo "Waiting for database to become available..."
  sleep 1
done
echo "PostgreSQL is up and running."

# Create the predefined user and database
echo "Creating new PostgreSQL user: $DB_USER"
psql -h localhost -d postgres -U $CURRENT_USER -c "CREATE USER $DB_USER WITH PASSWORD '$DB_PASS';"
echo "Creating new database: $DB_NAME"
createdb -h localhost -U $CURRENT_USER -O $DB_USER $DB_NAME



echo "PostgreSQL setup completed."