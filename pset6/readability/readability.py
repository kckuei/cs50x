from cs50 import get_string

# Get input text
input_str = get_string("Text: ")

# Count words, characters and sentences
count_words = 1
count_char = 0
count_sentences = 0
for c in input_str:
    if c in [".", "?", "!"]:
        count_sentences += 1
    if c in [" "]:
        count_words += 1
    if c.lower() >= 'a' and c.lower() <= 'z':
        count_char += 1

# print(count_char)
# print(count_words)
# print(count_sentences)

# Compute Coleman-Liau index
L = 100*count_char/count_words
S = 100*count_sentences/count_words
grade = 0.0588 * L - 0.296 * S - 15.8

# Print grades
if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    grade = int(round(grade))
    print(f"Grade {grade}")


