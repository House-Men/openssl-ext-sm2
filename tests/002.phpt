--TEST--
sm2_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('sm2')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = sm2_test1();

var_dump($ret);
?>
--EXPECT--
The extension sm2 is loaded and working!
NULL
