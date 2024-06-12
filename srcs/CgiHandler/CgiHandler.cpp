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

    _env["AUTH_TYPE"] = ;  // Type d'authentification utilisée
    _env["CONTENT_LENGTH"] = ;  // Longueur du corps de la requête
    _env["CONTENT_TYPE"] = ;  // Type de contenu du corps de la requête
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = ;  // Chemin de la ressource demandée
    _env["PATH_TRANSLATED"] = ;  // Chemin absolu traduit du système de fichiers
    _env["QUERY_STRING"] = ;  // Chaîne de requête
    _env["REMOTE_ADDR"] = ;  // Adresse IP du client
    _env["REMOTE_IDENT"] = ;  // Nom d'utilisateur pour l'identification distante
    _env["REMOTE_USER"] = ;  // Nom d'utilisateur pour l'authentification distante
    _env["REQUEST_METHOD"] = ;  // Méthode HTTP utilisée pour la requête
    _env["REQUEST_URI"] = ;  // URI de la requête
    _env["SCRIPT_NAME"] = ;  // Chemin du script CGI
    _env["SERVER_NAME"] = ;  // Nom du serveur
    _env["SERVER_PORT"] = ;  // Port sur lequel le serveur écoute
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

	int saveStdin = dup(STDIN_FILENO);
	int saveStdout = dup(STDOUT_FILENO);

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
		return "500";
	}
	else if (!pid)
	{
		char **a = NULL;
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(Script.c_str(), a, env);
		write(STDERR_FILENO, "500", 4);
	}
	else
	{
		char	buffer[BUFFER] = {NULL};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, BUFFER);
			ret = read(fdOut, buffer, BUFFER - 1);
			newbody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (unsigned int i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	return newbody;
}
