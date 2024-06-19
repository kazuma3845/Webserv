#include "Handler.hpp"

Handler::Handler(Request &request, Response &response) : _request(request), _response(response)
{
	_methodFunctions["GET"] = &Handler::handleGet;
	_methodFunctions["POST"] = &Handler::handlePost;
	_methodFunctions["DELETE"] = &Handler::handleDelete;
}

Handler::~Handler()
{
	// Destructor implementation
}

// ------------------------------------------------------ FUNCTIONS //

void Handler::start()
{
	std::map<std::string, void (Handler::*)()>::iterator it = _methodFunctions.find(_request.getMethod());
	if (it != _methodFunctions.end())
		(this->*(it->second))();
	else
		throw unknownMethod(501);
}

void Handler::handlePost()
{
	std::string contentType = _request.getHeaders()["Content-Type"];
	if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::string postData = _request.getBody();
		std::string outputPath = "Page/data/post_data.txt"; // Chemin prédéfini
		std::ofstream outFile(outputPath.c_str(), std::ios::app);
		if (!outFile.is_open())
			throw postFailed(500);
		outFile << postData << std::endl;
		outFile.close();
		_response.setStatusCode(201);
		_response.setContentType("text/plain");
		_response.setBody("POST data received and processed.");
	}
	else if (contentType.find("multipart/form-data") != std::string::npos)
	{
		std::string boundary = extractBoundary(contentType);
		std::istringstream stream(_request.getBody());
		std::string partContent;
		bool hasSuccessfulPart = false;

		// Lire et traiter chaque partie
		while (getNextPart(stream, boundary, partContent))
		{
			if (processPart(partContent))
			{
				hasSuccessfulPart = true;
			}
			else
			{
				throw postFailed(500); // Échec lors du traitement d'une des parties
			}
		}
		if (hasSuccessfulPart)
		{
			_response.setStatusCode(201);
			_response.setContentType("text/plain");
			_response.setBody("Files uploaded successfully.");
		}
		else
		{
			throw postFailed(500); // Aucun fichier n'a été traité avec succès
		}
	}
	else
	{
		throw postFailed(415); // Unsupported Media Type
	}
}

bool Handler::getNextPart(std::istringstream &stream, const std::string &boundary, std::string &partContent)
{
	partContent.clear();
	std::string line;
	bool inPart = false;

	while (std::getline(stream, line))
	{
		if (line.find(boundary) != std::string::npos)
		{
			if (inPart)
			{
				// Si on est déjà dans une partie, on retourne car on a atteint la fin de la partie
				return true;
			}
			// Commencer une nouvelle partie
			inPart = true;
		}
		else if (inPart)
		{
			if (!line.empty())
				partContent += line + "\n";
		}
	}
	return !partContent.empty();
}

std::string Handler::extractBoundary(const std::string &contentType)
{
	std::size_t pos = contentType.find("boundary=");
	if (pos != std::string::npos)
		return contentType.substr(pos + 9); // 9 pour passer 'boundary='
	return "";
}

bool Handler::processPart(const std::string &partContent)
{
    std::istringstream stream(partContent);
    std::string line;
    std::string disposition;
    std::string filename;

    while (std::getline(stream, line))
    {
        std::size_t pos = line.find("filename=");
        if (pos != std::string::npos)
        {
            filename = line.substr(pos + 10);                      // 10 pour passer 'filename="'
            filename = filename.substr(0, filename.length() - 1);  // Supprimer le dernier caractère (qui pourrait être une citation)

            // Ouvrir le fichier pour écrire
            std::string outputPath = "Page/data/" + filename;
            std::ofstream outFile(outputPath.c_str(), std::ios::binary);
            if (!outFile.is_open())
                return false;
            
            // Lire et écrire les données jusqu'à la prochaine boundary
            while (std::getline(stream, line))
            {
                if (line.find("--") == std::string::npos)
                    outFile << line << std::endl;
            }
            outFile.close();
            return true;
        }
    }
    return false;
}


void Handler::handleGet()
{
	std::string filePath = _request.getFilePath();
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1);

	if (extension == "html")
		_response.setContentType("text/html");
	else if (extension == "css")
		_response.setContentType("text/css");
	else if (extension == "js")
		_response.setContentType("application/javascript");
	else if (extension == "json")
		_response.setContentType("application/json");
	else if (extension == "jpg" || extension == "jpeg")
		_response.setContentType("image/jpeg");
	else if (extension == "png")
		_response.setContentType("image/png");
	else
		_response.setContentType("text/plain");
	_response.loadContent(_request.getFullPath());
	_response.setStatusCode(200);
}

void Handler::handleDelete()
{
	std::string filePath = _request.getFullPath();
	if (std::remove(filePath.c_str()) != 0)
		throw deletionFailed(500);
	_response.setStatusCode(204);
	_response.setContentType("text/plain");
	_response.setBody("Content deleted successfully");
}

// ------------------------------------------------------ ERRORS //

const char *Handler::unknownMethod::what() const throw()
{
	return ("Unhandled method.");
}
const char *Handler::deletionFailed::what() const throw()
{
	return ("DELETE failed.");
}

const char *Handler::postFailed::what() const throw()
{
	return ("POST processing failed.");
}