<?php
// Start the session
session_start();

// Unset specific session variables
unset($_SESSION['user_id']);

// End the session and destroy all data
session_destroy();
?>