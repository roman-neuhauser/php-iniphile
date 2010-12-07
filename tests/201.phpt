--TEST--
from_string with null input succeeds
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = iniphile::from_string(null);
echo "ok\n";
echo get_class($ini);

?>
--EXPECT--	
ok
iniphile
