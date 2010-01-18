--TEST--
syntax with comments, parsing test
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $q)
{
    var_dump($ini->get($q, array()));
}

$ini = new iniphile(dirname(__FILE__) . "/quirks.ini");

c($ini, 'first.first');
c($ini, 'first.second');
c($ini, 'first.third');
echo "\n";
c($ini, 'second.empty');
c($ini, 'second.emptywithcomment1');
c($ini, 'second.emptywithcomment2');
c($ini, 'second.multicomment');

?>
--EXPECT--	
array(1) {
  [0]=>
  string(4) "this"
}
array(1) {
  [0]=>
  string(4) "that"
}
array(1) {
  [0]=>
  string(15) "everything else"
}

array(0) {
}
array(0) {
}
array(0) {
}
array(2) {
  [0]=>
  string(5) "word1"
  [1]=>
  string(8) "q1 q2 q3"
}
