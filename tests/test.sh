#!/bin/bash
echo "<:ShareBooks client test:>-------|press enter to continue..."
read
echo "1.perform regular borrow book test:"
echo "-----------------------------------"
cp borrow_test.jpg ../face_cam/borrow_test.jpg
cp borrow_test.txt ../ISBN_scan/borrow_test.txt
echo "files copied to destination,go to main program start operation"
echo "when borrow test finished,press enter to continue..."
read
echo "2.perform regular return book test:"
echo "-----------------------------------"
rm ../face_cam/borrow_test.jpg
rm ../ISBN_scan/borrow_test.txt
cp return_test.jpg ../face_cam/return_test.jpg
cp return_test.txt ../ISBN_scan/return_test.txt
echo "files copied to destination,go to main program start operation"
echo "when return test finished,press enter to continue..."
read
echo "3.perform regular share book test:"
echo "----------------------------------"
rm ../face_cam/return_test.jpg
rm ../ISBN_scan/return_test.txt
cp share_test.jpg ../face_cam/share_test.jpg
cp share_test.txt ../ISBN_scan/share_test.txt
echo "files copied to destination,go to main program start operation"
echo "when share test finished,press enter to continue..."
read
echo "4.perform non-exist stuff test:"
echo "-------------------------------"
rm ../face_cam/share_test.jpg
rm ../ISBN_scan/share_test.txt
cp none_stuff.jpg ../face_cam/none_stuff.jpg
echo "files copied to destination,go to main program start operation"
echo "when non-exist stuff test finished,press enter to continue..."
read
echo "5.perform multi-face in camera test:"
echo "------------------------------------"
rm ../face_cam/none_stuff.jpg
cp multi_face_test.jpg ../face_cam/multi_face_test.jpg
echo "files copied to destination,go to main program start operation"
echo "when multi-face in camera test finished,press enter to continue..."
read
echo "6.perform quota used up test:"
echo "-----------------------------"
rm ../face_cam/multi_face_test.jpg
cp used_up_test.jpg ../face_cam/used_up_test.jpg
echo "files copied to destination,go to main program start operation"
echo "when quota used up test finished,press enter to continue..."
read
rm ../face_cam/used_up_test.jpg
echo "all test cases finished!press enter to exit."
read

