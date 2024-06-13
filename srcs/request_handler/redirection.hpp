#pragma once

#include <iostream>
#include <ctime>
#include <cstring>
#include "autoindex.hpp"
#include "../request/request.hpp"
#include "CgiHandler.hpp"

// Déclaration anticipée de la classe Response
class Response;

class Redirection
{
    private:
        std::string _body; //FIXME: A SUPPRIMER

        //check
        bool checkCgiExt(std::string uri, Request &a);
        bool checkMimeExt(std::string uri);

        //additional
        std::string takeTime();
        bool checkfolder(std::string uri);
        void check_ext_cgi(Request &a, Response &resp);
        void folderpath(Request &a, Response &resp);
    public:
        Redirection(){};
        ~Redirection(){};

        void path(Request &a, Response &resp);

        void reponseCGI(Request &a); //FIXME: A SUPPRIMER
        //get
        std::string getRep() const; //FIXME: A SUPPRIMER

        class Forbidden : public ErrorWebServ
        {
            public:
                Forbidden(int errorCode) : ErrorWebServ(errorCode) {}
                const char *what() const throw();
        };

        class UnsupportedMediaType : public ErrorWebServ
        {
            public:
                UnsupportedMediaType(int errorCode) : ErrorWebServ(errorCode) {}
                const char *what() const throw();
        };
};