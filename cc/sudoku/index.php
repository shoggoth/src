<?php

//
// Configuration for DB access. Alter these to suit your webhost's setup
//

//database name
$db_name = 'test';

//database host
$db_host = 'stein.scm.tees.ac.uk';

//database user name
$db_uname = 'u0019196';

//database password
$db_pass = '';

// table name
$table_name = 'sudoku';

// cookie name
$cookie_name = 'sudoku_solver';

//
// Please don't alter anything below here
// Unless you're sure you know what you're doing
//

// Connect to the database
//if (!connect_to_db($db_host, $db_uname, $db_pass, $db_name)) die('fuxored');

// make sure this doesn't get cached (HTTP 1.1)
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);

// Work out what's wanted
if ($_POST['solve'] ?? false) {
	
	$width = $_POST['width'];
	$height = $_POST['height'];
	$cellWidth = $_POST['cellWidth'];
	$cellHeight = $_POST['cellHeight'];
	$difficulty = $_POST['difficulty'];

	// Get the inputs from the POST
	$data_string = "";
	for ($i = 0; $i < $width * $height; $i++) $data_string .= $_POST["v$i"]." ";
	
	// Solve the puzzle
	$result = solve($width, $height, $cellWidth, $cellHeight, $data_string);

	if ($result == '') {
		echo "Invalid puzzle<br />\n";
	} else {
		$lines = explode("\n", $result);
		$cellValues = explode(' ', rtrim($lines[0]));
		echo(make_form($width, $height, $cellWidth, $cellHeight, $cellValues, $difficulty));
		for ($j = 1; $lines[$j]; $j++) echo $lines[$j]."<br />\n";
	}
	
} else if ($_POST['generate'] ?? false) {

	$difficulty = $_POST['difficulty'];
	
	if ($difficulty > 7) {
		$width = $height = 16;
		$cellWidth = $cellHeight = 4;
	} else if ($difficulty > 2) {
		$width = $height = 9;
		$cellWidth = $cellHeight = 3;
	} else {
		$width = 6; $height = 6;
		$cellWidth = 3; $cellHeight = 2;
	}

	// Generate an empty puzzle (all values zero)
	$data_string = "";
	for ($i = 0; $i < $width * $height; $i++) $data_string .= "0 ";

	// Solve the empty puzzle
	$result = solve($width, $height, $cellWidth, $cellHeight, $data_string);

	// Strip off useless data
	$lines = explode("\n", $result);
	$cellValues = explode(' ', rtrim($lines[0]));

	// Remove some values
	$totalValues = $width * $height;
	for ($i = 0; $i < $totalValues; $i++) {

		if (rand(0, 12) <= $difficulty) $cellValues[$i] = '0';
	}

	// Show the puzzle
	echo make_form($width, $height, $cellWidth, $cellHeight, $cellValues, $difficulty);
	
} else echo(make_form(9, 9, 3, 3));

function make_form($width, $height, $cellWidth, $cellHeight, $cellValues = null, $difficulty = 1) {
	
	global		$PHP_SELF;
	
	// The form
	$form  = "<form method=POST action=\"$PHP_SELF\">\n";
	
	// Hidden fields
	$form .= "<input type=\"hidden\" name=\"width\" value=\"$width\">\n";
	$form .= "<input type=\"hidden\" name=\"height\" value=\"$height\">\n";
	$form .= "<input type=\"hidden\" name=\"cellWidth\" value=\"$cellWidth\">\n";
	$form .= "<input type=\"hidden\" name=\"cellHeight\" value=\"$cellHeight\">\n";
	
	$form .= "<table>\n<tr>\n";
	$form .= "<td>\nDifficulty : <select name=\"difficulty\">\n";
	$form .= ($difficulty == 1) ? "<option value=\"1\" selected>The Sun</option>\n" : "<option value=\"1\">The Sun</option>\n";
	$form .= ($difficulty == 2) ? "<option value=\"2\" selected>The Daily Express</option>\n" : "<option value=\"2\">The Daily Express</option>\n";
	$form .= ($difficulty == 3) ? "<option value=\"3\" selected>The Daily Mail</option>\n" : "<option value=\"3\">The Daily Mail</option>\n";
	$form .= ($difficulty == 4) ? "<option value=\"4\" selected>The Mirror</option>\n" : "<option value=\"4\">The Mirror</option>\n";
	$form .= ($difficulty == 5) ? "<option value=\"5\" selected>Computer Weakly</option>\n" : "<option value=\"5\">Computer Weakly</option>\n";
	$form .= ($difficulty == 6) ? "<option value=\"6\" selected>The Guardian</option>\n" : "<option value=\"6\">The Guardian</option>\n";
	$form .= ($difficulty == 7) ? "<option value=\"7\" selected>The Times</option>\n" : "<option value=\"7\">The Times</option>\n";
	$form .= ($difficulty == 8) ? "<option value=\"8\" selected>Osaka Newspaper Concern</option>\n" : "<option value=\"8\">Osaka Newspaper Concern</option>\n";
	$form .= ($difficulty == 9) ? "<option value=\"9\" selected>Zeta Reticuli News</option>\n" : "<option value=\"9\">Zeta Reticuli News</option>\n";
	$form .= "</select>\n</td>\n";
	$form .= "<td>\n<input type=\"submit\" name=\"generate\" value=\"Generate\">\n</td>\n";
	$form .= "</tr>\n</table>\n";
	
	$form .= "<table border=\"1\">\n<tr>";
	
	for ($cells = $width * $height, $i = 0; $i < $cells; $i++) {
		
		$value = ($cellValues != null) ? $cellValues[$i] : '0';
		$form .= "<td><input type=\"text\" name=\"v$i\" value=\"$value\" size=\"2\" maxlength=\"2\"></td>";
		if (!(($i + 1) % $width) && $i < $cells - 1) $form .= "</tr>\n<tr>";
	}
	
	$form .= "</tr>\n</table>\n";
	
	$form .= "<table>\n<tr>\n";
	$form .= "<td>\n<input type=\"submit\" name=\"solve\" value=\"Solve\">\n</td>\n";
	$form .= "<td>\n<input type=\"submit\" name=\"clear\" value=\"Clear\">\n</td>\n";
	$form .= "</tr>\n</table>\n";
	$form .= "</form>\n";

	return $form;
}

function solve($width, $height, $cellWidth, $cellHeight, $data_string) {
	
	$process_streams = array(
	   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
	   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
	);
	
	if ($proc = proc_open('./sudoku.bin 2>&1', $process_streams, $pipes)) {
		
		// Write the dimensions to the process
		fwrite($pipes[0], "dims $width $height $cellWidth $cellHeight\n");
		
		// Write the data to the process
		fwrite($pipes[0], $data_string);
		fclose($pipes[0]);
		
		// Get the result
		$result = "";
		while (!feof($pipes[1])) $result .= fgets($pipes[1], 4096);
		fclose($pipes[1]);
		
		if (proc_close($proc)) $result = '';
	}

	return $result;
}

function connect_to_db($db_host, $db_uname, $db_pass, $db_name) {

	$connection = mysql_connect($db_host, $db_uname, $db_pass);
	$db = mysql_select_db($db_name);

	return ($connection && $db);

}

function mysql_one_array($query) {

	$one = mysql_query($query);
	$r = mysql_fetch_array($one);
	if(!$r) echo mysql_errno(). ": ".mysql_error(). "<br>";
	
	return($r);
}

function mysql_one_data($query) {

	$one = mysql_query($query);
	$r = mysql_fetch_row($one);
	if(!$r) echo mysql_errno(). ": ".mysql_error(). "<br>";
	
	return $r[0];
}

?>

<div><a href="sudoku.cc">source</a></div>
