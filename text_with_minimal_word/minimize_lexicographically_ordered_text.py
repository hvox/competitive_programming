def get_min_text(letters, number_of_words, k):
    letters = "".join(sorted(letters))
    words = [""] * number_of_words
    word_length = len(letters) // number_of_words
    for i in range(k):
        words[i] = letters[i]
    used_letters = k
    if k > 1:
        while len(words[k - 1]) < word_length and words[k - 2] == words[k - 1]:
            j = k - 1
            while j and words[j - 1] == words[k - 1]:
                j -= 1
            for i in range(j, k):
                words[i] += letters[used_letters]
                used_letters += 1
    delta = word_length - len(words[k - 1])
    words[k - 1] += letters[used_letters : used_letters + delta]
    used_letters += delta
    for i, word in enumerate(words):
        if len(word) != word_length:
            delta = word_length - len(word)
            words[i] = word + letters[used_letters : used_letters + delta]
            used_letters += delta
    return words


number_of_words = int(input("Number of words in the text: "))
word_length = int(input("Length of words: "))
k = int(input("Number of word to be minimized: "))
letters = input("allowed letters: ")
text = get_min_text(letters, number_of_words, k)
print(" ".join(text))
