--TEST--
basic syntax without comments, parsing smoketest
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile(dirname(__FILE__) . "/easy.ini");
echo "ok";

?>
--EXPECT--	
ok
