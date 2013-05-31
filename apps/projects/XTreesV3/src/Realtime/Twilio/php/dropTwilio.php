<?
header('Content-Type: text/xml');
echo '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>';
$connessione = mysql_connect("localhost:/tmp/mysql.sock","root","");
//print ("Connesso con successo"); 
mysql_select_db("twilio", $connessione);
$query = mysql_query("SELECT * FROM MESSAGES WHERE 1=1",$connessione);
if ($query==FALSE) die("errore nella composizione della query");
mysql_close($connessione);

$xml_output = "<messages>\n";

while($row=mysql_fetch_array($query)){
	$xml_output .= "\t<message>\n";
	$xml_output .= "\t\t<msg_id>";
	$xml_output .= $row[id];
	$xml_output .= "</msg_id>\n";
	$xml_output .= "\t\t<msg_data>";
	$xml_output .= utf8_encode($row[message]);
	$xml_output .= "</msg_data>\n";
	$xml_output .= "\t</message>\n";
}
$xml_output .= "\n</messages>";
echo $xml_output;
?>