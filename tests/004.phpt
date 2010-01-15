--TEST--
iniphile::get() called with argc != 2 throws
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c(iniphile $ini, array $args)
{
    echo sizeof($args);
    try {
        call_user_func_array(array($ini, 'get'), $args);
        echo ' ok';
    } catch (Exception $e) {
        echo ' fail';
    }
    echo "\n";
}

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

c($ini, array());
c($ini, array('foo'));
c($ini, array('foo', 'bar'));
c($ini, array('foo', 'bar', 'baz'));

?>
--EXPECT--	
0 fail
1 fail
2 ok
3 fail
