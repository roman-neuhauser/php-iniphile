--TEST--
construction with empty file succeeds
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");
echo "ok";

?>
--EXPECT--	
ok
