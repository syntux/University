import string
import sys
import math
import os

alphabet = string.ascii_lowercase
alphabetUpper = string.ascii_uppercase

def caesarEncrypt(text, key):
	output = ''
	for letter in text:
		if letter.isalpha():
			if letter.islower():
				index = (alphabet.find(letter) + key) % 26
				output += alphabet[index]
			else:
				index = (alphabetUpper.find(letter) + key) % 26
				output += alphabetUpper[index]
		else:
			output += letter

	return output

def caesarDecrypt(text, key):
	output = ''
	for letter in text:
		if letter.isalpha():
			if letter.islower():
				index = (alphabet.find(letter) - key) % 26
				output += alphabet[index]
			else:
				index = (alphabetUpper.find(letter) - key) % 26
				output += alphabetUpper[index]
		else:
			output += letter

	return output



def caesar(which, text, key):
	if len(key) == 1:
		if key.isalpha():
			key = alphabet.find(key)
		else:
			key = alphabetUpper.find(key)
	if which == 'e' or which == 'E':
		output = caesarEncrypt(text, key)
	elif which == 'd' or which =='D':
		output = caesarDecrypt(text, key)

	return output

def monoalphaEncrypt(text, key):
	letterList = []
	for letter in key:
		if letter not in letterList:
			letterList.append(letter)

	for letter in alphabet:
		if letter not in letterList:
			letterList.append(letter)
	letterListUpper = list(map(lambda x: x.upper(), letterList))

	output = ''
	for letter in text:
		if letter.isalpha():
			if letter.islower():
				index = alphabet.find(letter)
				output += letterList[index]
			else:
				index = alphabetUpper.find(letter)
				output += letterListUpper[index]
		else:
			output += letter

	return output

def monoalphaDecrypt(text, key):
	letterList = []
	for letter in key:
		if letter not in letterList:
			letterList.append(letter)

	for letter in alphabet:
		if letter not in letterList:
			letterList.append(letter)
	letterListUpper = list(map(lambda x: x.upper(), letterList))

	output = ''
	for letter in text:
		if letter.isalpha():
			if letter.islower():
				index = letterList.index(letter)
				output += alphabet[index]
			else:
				index = letterListUpper.index(letter)
				output += alphabetUpper[index]
		else:
			output += letter

	return output

def monoalpha(which, text, key):
	if len(key) > 6:
		print("Needs to be at least length of 6")
		return ''
	key = key.lower()
	if which == 'e' or which == 'E':
		output = monoalphaEncrypt(text, key)
	elif which == 'd' or which =='D':
		output = monoalphaDecrypt(text, key)

	return output


def polyalphaEncrypt(text, key):
	keyLen = len(key)

	count = 0
	output = ''
	for letter in text:
		result = caesar('e', letter, key[count % keyLen])
		output += result
		count += 1

	return output

def polyalphaDecrypt(text, key):
	keyLen = len(key)

	count = 0
	output = ''
	for letter in text:
		result = caesar('d', letter, key[count % keyLen])
		output += result
		count += 1
		
	return output

def polyalpha(which, text, key):
	if len(key) > 3:
		print("Needs to be at least length of 6")
		return ''
	key = key.lower()
	if which == 'e' or which == 'E':
		output = polyalphaEncrypt(text, key)
	elif which == 'd' or which =='D':
		output = polyalphaDecrypt(text, key)

	return output

def tth(text):
	textLen = len(text)
	loopCount = math.ceil(textLen / 16)
	countWhile = 0

	runningCount = [0 for i in range(4)]
	textCount = 0

	while (countWhile < loopCount):
		block = []
		blockNum = []
		count = 0

		i = 0
		tmpText = ''
		while (i < 16 and textCount < textLen):
			if text[textCount].isalpha():
				tmpText += text[textCount].upper()
				i += 1
			textCount += 1
		
		tmpTextLen = len(tmpText)
		while (tmpTextLen < 16):
			tmpText += 'A'
			tmpTextLen += 1

		print(tmpText)
		for i in range (4):
			row = []
			rowNum = []
			for j in range(4):
				if tmpText[count].isalpha():
					row.append(tmpText[count])
					if tmpText[count].islower():
						rowNum.append(alphabet.find(tmpText[count]))
					else:
						rowNum.append(alphabetUpper.find(tmpText[count]))
				count += 1
			block.append(row)
			blockNum.append(rowNum)

		
		for i in range(4):
			for j in range(4):
				runningCount[i] += blockNum[j][i] 
			runningCount[i] %= 26

		print(block)
		print("\n")
		print(blockNum)
		print(runningCount)


		for i in range(0, 3):
			tmp = blockNum[i][i+1:] + blockNum[i][:i+1]
			#print(blockNum[i][i+1:] + blockNum[i][:i+1])
			blockNum[i] = tmp

		blockNum[3] = blockNum[3][::-1]
		print(blockNum)
		for i in range(4):
			for j in range(4):
				runningCount[i] += blockNum[j][i] 
			runningCount[i] %= 26

		print(runningCount)

		output = ''
		for index in runningCount:
			output += alphabetUpper[index]

		countWhile += 1

	return output


def main():
	while (True):
		userChoice = input("\ne -- to encrypt\nd -- to decrypt\nq -- to quit\n")
		userChoice = userChoice.lower()
		while(not(userChoice == 'e' or userChoice == 'd' or userChoice == 'q')):
			userChoice = input("\n--ERROR. Wrong input.--\ne -- to encrypt\nd -- to decrypt\nq -- to quit\n")
			userChoice = userChoice.lower()


		if userChoice == 'e':
			userCipher = input('\nc -- Caesar Cipher\nm -- Monoaplhabetic Cipher\np -- Polyalphabetic Cipher\nt -- TTH\n')
			userCipher = userCipher.lower()

			while(not(userCipher == 'c' or userCipher == 'm' or userCipher == 'p' or userCipher == 't')):
				userCipher = input('\n--ERROR. Wrong input.--\nc -- Caesar Cipher\nm -- Monoaplhabetic Cipher\np -- Polyalphabetic Cipher\nt -- TTH\n')
				userCipher = userCipher.lower()

			if userCipher == 'c':
				userKey = input('\nEnter key a-z: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while(not(userKeyLen == 1) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key a-z: ')
					userKey = userKey.lower()
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = caesar('e', lines, userKey)
					print(f"The result is: {result}")

			elif userCipher == 'm':
				userKey = input('\nEnter key word: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while (not(userKeyLen > 0 and userKeyLen <= 6) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key word of at least size 6: ')
					userKey = userKey.lower()
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = monoalpha('e', lines, userKey)
					print(f"The result is: {result}")

			elif userCipher == 'p':
				userKey = input('\nEnter key letters: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while (not(userKeyLen > 0 and userKeyLen <= 3) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key letters of at least size 3: ')
					userKey = userKey.lower()
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = polyalpha('e', lines, userKey)
					print(f"The result is: {result}")

			elif userCipher == 't':
				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = tth(lines)
					print(f"The result is: {result}")



		elif userChoice == 'd':
			userCipher = input('\nc -- Caesar Cipher\nm -- Monoaplhabetic Cipher\np -- Polyalphabetic Cipher\n')
			userCipher = userCipher.lower()

			while(not(userCipher == 'c' or userCipher == 'm' or userCipher == 'p')):
				userCipher = input('\n--ERROR. Wrong input.--\nc -- Caesar Cipher\nm -- Monoaplhabetic Cipher\np -- Polyalphabetic Cipher\n')
				userCipher = userCipher.lower()

			if userCipher == 'c':
				userKey = input('\nEnter key a-z: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while(not(userKeyLen == 1) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key a-z: ')
					userKey = userKey.lower()
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = caesar('d', lines, userKey)
					print(f"The result is: {result}")

			elif userCipher == 'm':
				userKey = input('\nEnter key word: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while (not(userKeyLen > 0 and userKeyLen <= 6) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key word of at least size 6: ')
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = monoalpha('d', lines, userKey)
					print(f"The result is: {result}")

			elif userCipher == 'p':
				userKey = input('\nEnter key letters: ')
				userKey = userKey.lower()
				userKeyLen = len(userKey)
				while (not(userKeyLen > 0 and userKeyLen <= 3) or not(userKey.isalpha())):
					userKey = input('\n--ERROR. Wrong key length.-- Enter key letters of at least size 3: ')
					userKeyLen = len(userKey)

				userFile = input('\nEnter a valid file: ')
				while (not(os.path.isfile(userFile))):
					userFile = input('\n--ERROR. Not valid.-- Enter a valid file: ')

				with open(userFile) as fd:
					lines = fd.read()
					result = polyalpha('d', lines, userKey)
					print(f"The result is: {result}")


		elif userChoice == 'q':
			print('\nDone.')
			return 0

if __name__ == '__main__':
	sys.exit(main())