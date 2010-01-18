--TEST--
dots in option names represent graph edges
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $q)
{
    var_dump($ini->get($q, array()));
}

$ini = new iniphile(dirname(__FILE__) . "/dots.ini");

c($ini, 'top1.one.two');
echo "\n";
c($ini, 'top1.one.three');
c($ini, 'top1.one.four');
echo "\n";
c($ini, 'top1.one.two.three');
c($ini, 'top1.one.two.four');

?>
--EXPECT--	
array(0) {
}

array(1) {
  [0]=>
  string(9) "four five"
}
array(1) {
  [0]=>
  string(14) "five six eight"
}

array(1) {
  [0]=>
  string(7) "2nd 3rd"
}
array(2) {
  [0]=>
  string(3) "4th"
  [1]=>
  string(3) "5th"
}
