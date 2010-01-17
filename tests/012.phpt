--TEST--
iniphile::get($q, $d) interpretation of data with array $d
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c(iniphile $ini, $q)
{
    var_dump($ini->get($q, array()));
}

$ini = new iniphile(dirname(__FILE__) . "/easy.ini");

c($ini, 'strings.first');
c($ini, 'strings.then');
c($ini, 'strings.later');
c($ini, 'numbers.int');
c($ini, 'numbers.float');
c($ini, 'booleans1.ok');
c($ini, 'booleans2.ok');
c($ini, 'booleans3.ok');

?>
--EXPECT--	
array(1) {
  [0]=>
  string(4) "this"
}
array(3) {
  [0]=>
  string(4) "that"
  [1]=>
  string(3) "and"
  [2]=>
  string(4) "that"
}
array(3) {
  [0]=>
  string(15) "everything else"
  [1]=>
  string(7) "without"
  [2]=>
  string(10) "exceptions"
}
array(1) {
  [0]=>
  string(2) "42"
}
array(1) {
  [0]=>
  string(3) "6.9"
}
array(2) {
  [0]=>
  string(4) "true"
  [1]=>
  string(5) "false"
}
array(2) {
  [0]=>
  string(3) "yes"
  [1]=>
  string(2) "no"
}
array(2) {
  [0]=>
  string(1) "1"
  [1]=>
  string(1) "0"
}
