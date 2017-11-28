<?
date_default_timezone_set('America/New_York');

// Get current values:
if(isset($_GET['current'])) {
  $file = "data.txt";
  $fp = fopen($file, "r") or die ("not good - could not open");
  $values = fgets($fp);
  fclose($fp);
  $data = explode(':', $values);
  $json = json_encode($data);
  echo $json;
}
// Set current values:
if(isset($_GET['all'])) {
  $values = $_GET['values'];
  $values = str_replace(',',':',$values);
  $fp = fopen("data.txt", "w") or die("failed, man");
  fwrite($fp, $values);
  echo $values;
}

?>
