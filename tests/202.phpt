--TEST--
from_string with empty input succeeds
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = iniphile::from_string('');
echo "ok\n";
echo get_class($ini);

?>
--EXPECT--	
ok
iniphile
