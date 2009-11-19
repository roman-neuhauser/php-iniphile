--TEST--
iniphile smoketest
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile("rofl.ini");
var_dump($ini->path());

echo "Done\n";
?>
--EXPECTF--	
string(8) "rofl.ini"
Done
