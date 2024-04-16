<?php
// Check if the form is submitted via POST method
// echo  $_SERVER["PHP_SELF"];
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve the data sent via POST
    $name = $_POST["name"];
    $email = $_POST["email"];
    
    // Display the received data
    echo "<h2>Received Data:</h2>";
    echo "Name: " . $name . "<br>";
    echo "Email: " . $email;
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Local POST Request</title>
</head>
<body>
    <h1>Local POST Request Example</h1>
    <form method="post">

        <label for="name">Name:</label>
        <input type="text" name="name" id="name"><br><br>
        <label for="email">Email:</label>
        <input type="" name="email" id="email"><br><br>
        <input type="submit" value="Submit">
    </form>
</body>
</html>
