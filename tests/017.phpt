--TEST--
syntax with comments, parsing smoketest
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile(dirname(__FILE__) . "/quirks.ini");
echo "ok";

?>
--EXPECT--	
ok
