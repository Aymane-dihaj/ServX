import cgi

print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<body>")

print('<form method="post" action="{}">'.format(cgi.escape(__file__)))
print('  Name: <input type="text" name="fname">')
print('  <input type="submit">')
print('</form>')

form = cgi.FieldStorage()

if form.getvalue("fname"):
    name = form.getvalue("fname")
    print(name)
else:
    print("Name is empty")

print("</body>")
print("</html>")
