#!/usr/bin/python3
def generate_http_response(prenom):
    html_content = f"""
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Bienvenue</title>
    </head>
    <body>
        <h1>Bienvenue {prenom} !</h1>
    </body>
    </html>
    """

    http_response = f"""HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: {len(html_content)}

{html_content}"""

    return http_response

prenom = "TonPrenom"  # Remplace par le prénom souhaité
response = generate_http_response(prenom)
print(response)
