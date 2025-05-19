#!/bin/bash
# Set as executable with `chmod +x install_xti.sh`

set -e # Exit on error

# Install destinations
BIN_DEST="/usr/local/bin"
LUA_DEST="/usr/local/share/xti"

# Detect shell 
if [ -n "$ZSH_VERSION" ]; then
    SHELL_CONFIG="$HOME/.zshrc"
elif [ -n "$BASH_VERSION" ]; then
    SHELL_CONFIG="$HOME/.bashrc"
fi

echo "Installing XtI..."

# Install xti binary
if [ ! -f ./bin/xti ]; then
    echo "Error: xti binary not found. Please run 'make' first."
    exit 1
fi

sudo cp ./bin/xti "$BIN_DEST"
echo "Installed xti to $BIN_DEST"

# Install Lua scripts
sudo mkdir -p "$LUA_DEST"
sudo cp ./src/lua/*.lua "$LUA_DEST"
echo "Copied Lua scripts to $LUA_DEST"

# Set env vars in shell
if ! grep -q "XTI_LUA_PATH" "$SHELL_CONFIG"; then
    echo "export XTI_LUA_PATH=$LUA_DEST" >> "$SHELL_CONFIG"
    echo "Added XTI_LUA_PATH to $SHELL_CONFIG"
    echo "Run 'source $SHELL_CONFIG' or restart your terminal to apply changes."
else
    echo "XTI_LUA_PATH already set in $SHELL_CONFIG"
fi

echo "Installation complete. Run `xti -h` for help"

