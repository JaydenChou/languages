#!/usr/bin/env python
# Program:    phonecody.py
# Type:       Python Script
#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
#
# References to the same problem in other languages:
# 	http://aarongraham.com/cplusplus/
# 	http://www.digitalmars.com/d/2.0/lisp-java-d.html

import sys;
import os;
import re;
import string;
import bisect;

# Task description for the "Phonecode" benchmark
# Consider the following mapping from characters to digits:
# We want to use this mapping for encoding telephone numbers by myWordDict, so that it becomes easier to remember the numbers.
# 
#     E | J N Q | R W X | D S Y | F T | A M | C I V | B K U | L O P | G H Z 
#     e | j n q | r w x | d s y | f t | a m | c i v | b k u | l o p | g h z 
#     0 |   1   |   2   |   3   |  4  |  5  |   6   |   7   |   8   |   9



# We want to use this mapping for encoding telephone numbers by words, so that it becomes easier to remember the numbers.
# Functional requirements
# Your task is writing a program that finds, for a given phone number, all possible encodings by words, and prints them. A phone number is an arbitrary(!) string of dashes (-), slashes (/) and digits. The dashes and slashes will not be encoded. The words are taken from a dictionary which is given as an alphabetically sorted ASCII file (one word per line).

# Only exactly each encoding that is possible from this dictionary and that matches the phone number exactly shall be printed. Thus, possibly nothing is printed at all. The words in the dictionary contain letters (capital or small, but the difference is ignored in the sorting), dashes (-) and double quotes ("). For the encoding only the letters are used, but the words must be printed in exactly the form given in the dictionary. Leading non-letters do not occur in the dictionary. (The quotes indicate Umlaut characters, but this fact is irrelevant.)

# Encodings of phone numbers can consist of a single word or of multiple words separated by spaces. The encodings are built word by word from left to right. If and only if at a particular point no word at all from the dictionary can be inserted, a single digit from the phone number can be copied to the encoding instead. Two subsequent digits are never allowed, though. To put it differently: In a partial encoding that currently covers k digits, digit k+1 is encoded by itself if and only if, first, digit k was not encoded by a digit and, second, there is no word in the dictionary that can be used in the partial encoding starting at digit k+1.

# Your program must work on a series of phone numbers; for each encoding that it finds, it must print the phone number followed by a colon, a single(!) space, and the encoding on one line; trailing spaces are not allowed.

# All remaining ambiguities in this specification will be resolved by the following example. (Still remaining ambiguities are intended degrees of freedom.)

# Dictionary (in file test.w):

# an
# blau
# Bo"
# Boot
# bo"s
# da
# Fee
# fern
# Fest
# fort
# je
# jemand
# mir
# Mix
# Mixer
# Name
# neu
# o"d
# Ort
# so
# Tor
# Torf
# Wasser
# 
# Phone number list (in file test.t):
# 
# 112
# 5624-82
# 4824
# 0721/608-4067
# 10/783--5
# 1078-913-5
# 381482
# 04824
# 
# Program start command (must have this form!):
# 
# python phonecode.py test.w test.t
# 
# Corresponding correct program output (to stdout):
# 
# 5624-82: mir Tor
# 5624-82: Mix Tor
# 4824: Torf
# 4824: fort
# 4824: Tor 4
# 10/783--5: neu o"d 5
# 10/783--5: je bo"s 5
# 10/783--5: je Bo" da
# 381482: so 1 Tor
# 04824: 0 Torf
# 04824: 0 fort
# 04824: 0 Tor 4
# 
# Any other output would be wrong (except for different ordering of the lines).

# Wrong outputs for the above example would be e.g.
# 562482: Mix Tor , because the formatting of the phone number is incorrect,
# 10/783--5: je bos 5 , because the formatting of the second word is incorrect,
# 4824: 4 Ort , because in place of the first digit the words Torf, fort, Tor could be used,
# 1078-913-5: je Bo" 9 1 da , since there are two subsequent digits in the encoding,
# 04824: 0 Tor , because the encoding does not cover the whole phone number, and
# 5624-82: mir Torf , because the encoding is longer than the phone number.
# 
# The above data are available to you in the files test.w (dictionary), test.t (telephone numbers) and test.out (program output).
# Quantitative requirements
# Length of the individual words in the dictionary: 50 characters maximum.
# Number of words in the dictionary: 75000 maximum
# Length of the phone numbers: 50 characters maximum.
# Number of entries in the phone number file: unlimited.
# Quality requirements
# 
# Work carefully and deliver a high quality program. In particular, thoroughly comment your source code (design ideas etc.).
# 
# The focus during program construction shall be on correctness. Generate exactly the right output format. Do not generate additional output. I will automatically test your program with possibly hundreds of thousands of phone numbers and it should not make a single mistake -- in particular it must not crash. Take youself as much time as is required to ensure correctness.
# 
# Your program must be run time efficient in so far that it analyzes only a small fraction of all dictionary entries in each word appending step. The dictionary must be read into main memory entirely, but you must not do the same with the phone number file, as that may be arbitrarily large.
# 
# Your program needs not be robust against incorrect formats of the dictionary file or the phone number file.


def main():
	if len(sys.argv) < 3:
		print "Usage: ";
		print "\t" + sys.argv[0] + " {dictionary_file} {telefone_numbers_file}";
		print "";
		print "Example: ";
		print "\t" + sys.argv[0] + " test.w test.t";
		print "";
		sys.exit(1);

	for fname in sys.argv:
		if not os.path.exists(fname):
			print "ERROR: " + "file " + fname + " does not exist.";
			sys.exit(1);
	# dictionary file
	dictFile = sys.argv[1];
	# phone file
	phoneFile = sys.argv[2];

	# Mapping from characters to digits:
	#     E | J N Q | R W X | D S Y | F T | A M | C I V | B K U | L O P | G H Z 
	#     e | j n q | r w x | d s y | f t | a m | c i v | b k u | l o p | g h z 
	#     0 |   1   |   2   |   3   |  4  |  5  |   6   |   7   |   8   |   9

	# And is the same:
	# ABCDEFGHIJKLMNOPQRSTUVWXYZ
	# abcdefghijklmnopqrstuvwxyz
	# 57630499617851881234762239

	myencodingmap = '57630499617851881234762239';
	number2WordTranslationTable = string.maketrans(string.uppercase+string.lowercase, myencodingmap*2);

	# 
	# read the dictionary file and build a list of numbers per word
	#
	myWordDict = {};

	for line in file(dictFile).read().splitlines():
		word = line.strip();
		number = word.translate(number2WordTranslationTable);
		numberList = [];
		if myWordDict.has_key(number):
			numberList = list(myWordDict[number]);
		numberList.append(word);
		myWordDict[number] = numberList;

	#
	# build a dictionary with [words] <-> [phonelines]
	#
	wordPhonesDict = {};
	for phoneLine in file(phoneFile).read().splitlines():
		# iterate keys
		for number,listWords in myWordDict.iteritems():
			# look number's keys into the phoneLine
			if number in phoneLine:
				# number is part of the phoneLine
				# Build a new word using the translatedWord of number
				for word in listWords:
					# this is the word, now we have to build the replaced phoneword
					phoneWordTmp = phoneLine.replace(number, ' '+word+' ').strip(' ');
					wordPhonesDict[phoneWordTmp] = phoneLine;
			# some keys have '"'
			if number.count('"'):
				number = number.replace('"', '');
				if number in phoneLine:
					# number is part of the phoneLine
					# Build a new word using the translatedWord of number
					for word in listWords:
						# this is the word, now we have to build the replaced phoneword
						phoneWordTmp = phoneLine.replace(number, ' '+word+' ').strip(' ');
						wordPhonesDict[phoneWordTmp] = phoneLine;


	# 
	# in this cicle we are changing the wordPhonesDict{} if there is more numbers to translate
	#

	# Main cicle
	change = True;
	sizeOut = len(wordPhonesDict)*2;
	while change == True and sizeOut > 0:
		sizeOut -= 1;
		change = False;
		wordPhonesDictTmp = wordPhonesDict;
		# Cicle in our temporary dictionary translatedphones (phoneword) <-> phones
		for phoneword, phone in wordPhonesDictTmp.iteritems():
			phoneWordTmp = phoneword.replace('-', '').replace('/', '');
			if re.compile(r'[0-9][0-9]').findall(phoneword):
				# iterate keys
				# Cicle in our Main dictionary numbers <-> words
				for number,listWords in myWordDict.iteritems():
					# look numberskeys into the phone
					if number in phoneWordTmp:
						# number is part of the phone
						# Build a new word using the translatedWord of number
						for word in listWords:
							# this is the word, now we have to build the phoneWordTmp
							newphoneword = phoneWordTmp.replace(number, ' '+word+' ').strip(' ').replace('  ', ' ');
							# deleting old words from the dict
							if wordPhonesDict.has_key(phoneword):
								wordPhonesDict.pop(phoneword);
							wordPhonesDict[newphoneword] = phone;
						# A change in the dict is made
						change = True;
			# Try again, there is a change
			if change == True:
				break;

	phonesWordDict = {};
	for w,p in wordPhonesDict.iteritems():
		wordList = [];
		if phonesWordDict.has_key(p):
			wordList = list(phonesWordDict[p]);
		wordList.append(w);
		phonesWordDict[p] = wordList;
		
	#
	# Controling Wrong Outputs
	#
	# Wrong outputs for the above example would be e.g.
	# 562482: Mix Tor , because the formatting of the phone number is incorrect,
	# 10/783--5: je bos 5 , because the formatting of the second word is incorrect,
	# 4824: 4 Ort , because in place of the first digit the words Torf, fort, Tor could be used,
	# 1078-913-5: je Bo" 9 1 da , since there are two subsequent digits in the encoding,
	# 04824: 0 Tor , because the encoding does not cover the whole phone number, and
	# 5624-82: mir Torf , because the encoding is longer than the phone number.
	for p,wl in phonesWordDict.iteritems():
		for w in wl:
			todelete = False;
			while True:
				# not allow 2 consecutive numbers
				if re.compile(r'[0-9][0-9]').findall(w):
					todelete = True;
					break;
				# controlling if there are more than one number in the word, or if the word begins with a number
				if len(re.compile(r'[0-9]').findall(w)) > 1 or re.compile(r'^[0-9]').findall(w):
					if len(wl) > 1:
						for w2 in wl:
							# if some w2 in the list begins with letters, delete this item
							if re.compile(r'^[a-zA-Z]').findall(w2):
								todelete = True;
								break;
				break;
						
			# This item must be deleted
			if todelete == True:
				wordPhonesDict.pop(w);
		
	#
	# print all the items in this final dictionary: wordPhonesDict
	#
	for w,p in wordPhonesDict.iteritems():
		print p + ': ' + w;

if __name__ == "__main__": 
	main();
