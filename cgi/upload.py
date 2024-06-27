#!/usr/bin/env python3
import os
import cgi
import cgitb
import shutil

cgitb.enable()

UPLOAD_DIR = "/Users/kazuma3845/Desktop/webserv/Page/data/"

def move_uploaded_file(fileitem, upload_dir):
    if not fileitem.file:
        return False

    # Assurez-vous que le dossier de destination existe
    os.makedirs(upload_dir, exist_ok=True)
    
    filepath = os.path.join(upload_dir, os.path.basename(fileitem.filename))
    
    with open(filepath, 'wb') as fout:
        shutil.copyfileobj(fileitem.file, fout)

    return True

def main():
    print("Content-Type: text/html")
    print()

    form = cgi.FieldStorage()
    fileitem = form['file']

    if fileitem.filename:
        success = move_uploaded_file(fileitem, UPLOAD_DIR)
        if success:
            message = f"Le fichier '{fileitem.filename}' a été déplacé avec succès."
        else:
            message = "Erreur lors du déplacement du fichier."
    else:
        message = "Aucun fichier téléchargé."

    print(f"""<!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Résultat du téléchargement</title>
        <style>
            body {{
                margin: 0;
                padding: 0;
                font-family: 'Arial', sans-serif;
                background-color: #282c34;
                color: white;
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                text-align: center;
            }}
            .container {{
                max-width: 800px;
                padding: 20px;
                background-color: #333;
                border-radius: 10px;
                box-shadow: 0 0 10px rgba(0,0,0,0.2);
            }}
            h1 {{
                font-size: 3em;
                margin-bottom: 0.5em;
            }}
            p {{
                font-size: 1.2em;
                margin-bottom: 1em;
            }}
        </style>
    </head>
    <body>
        <div class="container">
            <h1>Résultat du téléchargement</h1>
            <p>{message}</p>
        </div>
    </body>
    </html>""")

if __name__ == "__main__":
    main()
