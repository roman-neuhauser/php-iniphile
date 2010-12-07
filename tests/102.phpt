--TEST--
from_file() with empty file succeeds
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = iniphile::from_file(dirname(__FILE__) . "/empty.ini");
echo "ok\n";
echo get_class($ini);

?>
--EXPECT--	
ok
iniphile
