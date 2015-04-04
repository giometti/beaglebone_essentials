#!/bin/bash

# The server's root directory
base=/var/www

# Read the browser request
read request

# Now read the message header
while /bin/true; do
	read header
	[ "$header" == $'\r' ] && break;
done

# Parse the GET request
tmp="${request#GET }"
tmp="${tmp% HTTP/*}"

# Get the URL and replace it with "/index.html" in case it is set to "/"
url="${tmp%\?*}"
[ "$url" == "/" ] && url="/index.html"

# Extract the filename
filename="$base$url"
extension="${filename##*.}"

# Check for file exist
if [ -f "$filename" ]; then
	echo -e "HTTP/1.1 200 OK\r"
	echo -e "Contant-type: text/html\r"
	echo -e "\r"
	cat "$filename"
	echo -e "\r"
else
	# If the file does not exist return an error
	echo -e "HTTP/1.1 404 Not Found\r"
	echo -e "Content-Type: text/html\r"
	echo -e "\r"
	echo -e "404 Not Found\r"
	echo -e "Not Found
		The requested resource was not found\r"
	echo -e "\r"
fi

exit 0
