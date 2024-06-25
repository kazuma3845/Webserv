#!/usr/bin/env python3

import cgi
import cgitb
import os

# Activer le débogage pour voir les erreurs CGI dans le navigateur
cgitb.enable()

# Définir le répertoire de téléchargement
upload_dir = "/Users/kazuma3845/Desktop/webserv/Page/data/"

# Récupérer les données du formulaire CGI
form = cgi.FieldStorage()

# Vérifier si le fichier 'image' a été envoyé
if 'image' in form:
    # Obtenir les détails du fichier
    fileitem = form['image']

    # Vérifier si le fichier a bien été envoyé
    if fileitem.file:
        # Construire le chemin complet pour enregistrer le fichier
        filepath = os.path.join(upload_dir, os.path.basename(fileitem.filename))
        
        try:
            with open(filepath, 'wb') as f:
                f.write(fileitem.file.read())
            
            print("Content-Type: text/html\r\n\r\n")
            print("<html><body>")
            print("<h2>Upload réussi</h2>")
            print(f"<p>Fichier sauvegardé sous: {filepath}</p>")
            print("</body></html>")
        except IOError as e:
            print("Content-Type: text/html\r\n\r\n")
            print("<html><body> <CENTER> <BIG> <BIG> <BIG> <BIG> <BIG> <B> Error 403: Forbidden </B> </body></html>")
else:
    print("Content-Type: text/html\r\n\r\n")
    print("<html><body> <CENTER> <BIG> <BIG> <BIG> <BIG> <BIG> <B> Error 403: Forbidden </B> </body></html>")

