#include "CgiHandler.hpp"

CgiHandler::CgiHandler(Request &request) : _body(request.getBody())
{
	initenv(request);
}

CgiHandler::~CgiHandler()
{
}

void CgiHandler::initenv(Request &request)
{
    _env["AUTH_TYPE"] = request.getHeaders()["Authorization"];  // Type d'authentification utilisée
    _env["CONTENT_LENGTH"] = request.getBody().size();  // Longueur du corps de la requête
    _env["CONTENT_TYPE"] =request.getHeaders()["Content-Type"];  // Type de contenu du corps de la requête
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = request.getFilePath() + request.getCurr_loc().getIndex();  // Chemin de la ressource demandée
    _env["PATH_TRANSLATED"] = request.getFullPath();  // Chemin absolu traduit du système de fichiers
    // _env["QUERY_STRING"] = request.getQueryString();  // Chaîne de requête (after ? in url)
    // _env["REMOTE_ADDR"] = request.getHost();  // Adresse IP du client (host)
    _env["REMOTE_IDENT"] = request.getHeaders()["Authorization"];  // Nom d'utilisateur pour l'identification distante
    _env["REMOTE_USER"] = request.getHeaders()["Authorization"];  // Nom d'utilisateur pour l'authentification distante
    _env["REQUEST_METHOD"] = request.getMethod();  // Méthode HTTP utilisée pour la requête
    _env["REQUEST_URI"] = request.getURI();  // URI de la requête
    _env["SCRIPT_NAME"] = request.getFilePath() + request.getCurr_loc().getIndex();;  // Chemin du script CGI
    // _env["SERVER_NAME"] = request.getServName;  // Nom du serveur (name)
    // _env["SERVER_PORT"] = request.getPort;  // Port sur lequel le serveur écoute (listen)
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "Weebserv/1.0";
    _env["REDIRECT_STATUS"] = "200";
}

char **CgiHandler::EnvToArray() const
{
	char **array = new char*[_env.size() + 1];
	int j = 0;
	for (std::map<std::string, std::string>::const_iterator i = _env.begin(); i != _env.end(); i++)
	{
		std::string line = i->first + "=" + i->second;
		array[j] = new char[line.size() + 1];
		strcpy(array[j], line.c_str());
		j++;
	}
	array[j] = NULL;
	return array;
}

std::string CgiHandler::execute(std::string Script)
{
	char **env = EnvToArray();
	pid_t pid;
	std::string newbody;
	int fdin = dup(STDIN_FILENO);
	int fdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error Fork";
		throw InternalServerError(500);
	}
	else if (!pid)
	{
		char **tmp = NULL;

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(Script.c_str(), tmp, env);
		std::cerr << "Execution failed" << std::endl;
		write(STDOUT_FILENO, "Code 500\n", 10);
	}
	else
	{
		char	buffer[1000] = {0};
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1000);
			ret = read(fdOut, buffer, 1000 - 1);
			newbody += buffer;
		}		
	}

	dup2(fdin, STDIN_FILENO);
	dup2(fdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(fdin);
	close(fdout);

	for (unsigned int i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	if (newbody.compare("Code 500\n") == 0)
		throw InternalServerError(500);

	return newbody;
}

const char* CgiHandler::InternalServerError::what() const throw()
{
    return "Internal Server Error";
}