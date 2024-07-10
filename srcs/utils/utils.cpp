#include "utils.hpp"

// * Replaces all instances of double slashes ("//") with a single slash ("/") in a given string.

void replaceDoubleSlashes(std::string &str)
{
	std::string::size_type pos = 0;
	while ((pos = str.find("//", pos)) != std::string::npos)
		str.replace(pos, 2, "/");
}

bool isDirectory(std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
	{
		std::cerr << "Cannot access " << path << std::endl;
		return false;
	}
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;
}

std::string readOnce(int fd)
{
	size_t buffer_size = 4096;
	char buffer[buffer_size];
	ssize_t bytes_read = read(fd, buffer, buffer_size);
	if (bytes_read < 0)
		throw errorReadingFD(500);
	if (bytes_read == 0)
		throw connectionCloseEarly(499);
	std::string current_buffer(buffer, bytes_read);
	return current_buffer;
}

const char *connectionCloseEarly::what() const throw()
{
	return ("Connexion between client and server closed.");
}
const char *errorReadingFD::what() const throw()
{
	return ("Error while trying to read fd.");
}