#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <regex.h>

#define WORD_MAX_SIZE 30
#define CLASS_MAX_SIZE 30
#define DEFAULT_INPUT_FILE_NAME "input.txt"
#define NUMBER "NUMBER"
#define CONSTANT "CONSTANT"
#define DIRECTIVE "DIRECTIVE"
#define COMMENT "COMMENT"
#define KEYWORD "KEYWORD"
#define OPERATOR "OPERATOR"
#define EMPTY "EMPTY"
#define PUNCTUATION_SIGN "PUNCTUATION_SIGN"
#define IDENTIFICATOR "IDENTIFICATOR"
#define ERROR "ERROR"



int match(const char *string, const char *pattern)
{
regex_t re;
if (regcomp(&re, pattern, REG_EXTENDED) != 0) return 0;
int status = regexec(&re, string, 0, NULL, 0);
regfree(&re);
if (status != 0) return 0;
return 1;
}

int isDigit(char *word, int wordMaxSize){
    const char* re = "^([-+]?([0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?)|(((0)|(&[oO][0-7]*)|(&[bB][0-1]*)|(&[hH][0-9A-F]*)|([1-9][0-9]*))([SILDFR]|US|UI|UL)?))$";
    char str[wordMaxSize];
    strcpy(str, word);
    return match(str, re);
}

int isIdentificator(char *word, int wordMaxSize){
    const char* re = "^([a-zA-Z][a-zA-Z0-9_]*[%&!#$@]?)$";
    char str[wordMaxSize];
    strcpy(str, word);
    return match(str, re);
}

int isClass(char *word, char** arr, int arrCount){
    
    for(int k = 0; k < arrCount; k++){
        if(strcmp(word, arr[k]) == 0){
            return 1;
        }
    }
    return 0;
}

char** doubleArraySize(char** arr, int oldSize) {
    int doubledSize = 2 * oldSize;
    char** doubledArr = (char**)malloc(doubledSize * sizeof(char*));
    for (long i = 0; i < oldSize; i++) {
        if(arr[i] != NULL){
            doubledArr[i] = strdup(arr[i]);
        }
    }
    for (long i = 0; i < oldSize; i++) {
        if(arr[i] != NULL){
            free(arr[i]);
        }
    }
    free(arr);
    return doubledArr;
}

char* doubleWordSize(char* arr, int oldSize) {
    int doubledSize = 2 * oldSize;
    char* doubledArr = (char*)malloc(doubledSize * sizeof(char));
    for (int i = 0; i < oldSize; i++) {
        doubledArr[i] = arr[i];
    }
    free(arr);
    return doubledArr;
}

void addWord(char **words, char *word, int *index, int *symbolsCount, int wordMaxSize){
    if(&symbolsCount > 0){
        words[*index] = (char *)malloc(sizeof(char)*(wordMaxSize));
        words[*index] = strdup(word);
        memset(word,0,wordMaxSize);
        *index = *index + 1;
        *symbolsCount = 0;
    }
}
void addClass(char **classes, char *class, int *index, int classMaxSize){
        classes[*index] = (char *)malloc(sizeof(char)*(classMaxSize));
        classes[*index] = strdup(class);
        *index = *index + 1;
}

void addLexeme(char **words, char *tmp, int *i, int *j, int wordMaxSize, char **classes, int *m, int classMaxSize, int *wordCount){
    addWord(words, tmp, i, j, wordMaxSize);
    char *keywords[] = {
        "AddHandler",    "AddressOf",   "Alias",    "And",    "As",
        "Boolean",    "ByRef",    "Byte",    "ByVal",    "Call",    "Case",
        "Catch",    "CBool",    "CByte",   "CChar",    "CDate",    "CDbl",
        "CDec",    "Char",   "CInt",    "Class",    "CLng",    "CObj",
        "Const",    "Continue",    "CSByte",    "CShort",    "CSng",    "CStr",
        "CType",    "CUInt",    "CULng",    "CUShort",    "Date",    "Decimal",
        "Declare",    "Default",    "Delegate",    "Dim",    "DirectCast",    "Do",
        "Double",    "Each",    "Else",    "ElseIf",    "End",    "EndIf",
        "Enum",    "Erase",    "Error",    "Event",    "Exit",    "False",
        "Finally",    "For",    "For", "Each…Next",    "Friend",    "Function",   "Get",
        "GetType",    "GetXMLNamespace",    "Global",    "GoSub",    "GoTo",    "Handles",
        "If",    "Implements",    "Imports",    "In",    "Inherits",    "Integer",
        "Interface",    "Is",    "Let",    "Lib",    "Like",
        "Long",    "Loop",   "Me",    "Module",    "MustInherit",
        "MustOverride",    "MyBase",    "MyClass",    "Namespace",    "Narrowing",    "New",
        "Next",    "Not",    "Nothing",    "NotInheritable",    "NotOverridable",    "Object",
        "Of",    "On",    "Operator",    "Option",    "Optional",    "Or",
        "Out",    "Overloads",    "Overridable",    "Overrides",    "ParamArray",
        "Partial",    "Private",    "Property",    "Protected",    "Public",    "RaiseEvent",
        "ReadOnly",    "ReDim",    "REM",    "RemoveHandler",    "Resume",    "Return",
        "SByte",    "Select",    "Set",    "Shadows",    "Shared",    "Short",
        "Single",    "Static",    "Step",    "Stop",    "String",    "Structure",
        "Sub",    "SyncLock",    "Then",    "Throw",    "To",    "True",
        "Try",    "TryCast",    "TypeOf…Is",    "UInteger",    "ULong",    "UShort",
        "Using",    "Variant",    "Wend",    "When",    "While",    "Widening",
        "With",    "WithEvents",   "WriteOnly"    "Xor"            };
    int keywordCount = 150;
    
    char *operators[] = {
        "+", "-", "*", "/", "\\", "Mod", "^",
        "=", "+=", "-=", "*=", "/=", "\\=", "^=",
        "&=", ">>=", "<<=", "And", "Or", "Not", "Xor",
        "AndAlso", "OrElse", "IsFalse", "IsTrue", "<", "<=", ">",
        ">=", "<>", "Is", "IsNot", "&", "+"
    };
    int operatorsCount = 34;
    
    char *directives[] = {
        "#Const", "#Else", "#ElseIf", "#End", "#If", "#ExternalSource", "#Region"
    };
    int directivesCount = 7;
    if(isClass(words[*m], directives, directivesCount)){
        addClass(classes, DIRECTIVE, m, classMaxSize);
    }else if(isClass(words[*m], operators, operatorsCount)){
        addClass(classes, OPERATOR, m, classMaxSize);
    }else if(isClass(words[*m], keywords, keywordCount)){
        addClass(classes, KEYWORD, m, classMaxSize);
    }else if(isDigit(words[*m], wordMaxSize)){
        addClass(classes, NUMBER, m, classMaxSize);
    }else if(isIdentificator(words[*m], wordMaxSize)){
        addClass(classes, IDENTIFICATOR, m, classMaxSize);
    }else{
        addClass(classes, ERROR, m, classMaxSize);
    }
    (*wordCount)++;
}

char** parseWordsFromFile(char **words, char ***classes, FILE *fp, int wordMaxSize, int wordArraySize, int classMaxSize, int classArraySize, int *wordCount){
    char punctuationSigns[] = {
        '{', '}', '[', ']', ',', ';', '(', ')', '.'
    };
    int punctuationSignsCount = 9;
    char empty[] = {'\n','\r','\t','\v','\r','\b', '\0', ' '};
    int emptyCount = sizeof(empty)/sizeof(char);
    char c;
    int i = 0, j = 0, k = 0, m = 0;
    int isSeparator = 0, isPunctuator;
    char *tmp = (char*)malloc(wordMaxSize * sizeof(char));;
    memset(tmp,0,wordMaxSize);
    if(fp)
    {
        while((c = fgetc(fp)) != EOF)
        {
            isSeparator = 0;
            isPunctuator = 0;
            for(k = 0; k < emptyCount; k++){
                if(c == empty[k]){
                    isSeparator = 1;
                    k = 0;
                    break;
                }
            }
            if(!isSeparator){
                for(k = 0; k < punctuationSignsCount; k++){
                    if(c == punctuationSigns[k]){
                        isPunctuator = 1;
                        k = 0;
                        break;
                    }
                }
            }
            if(c == '\''){
                if(j > 0 && j <= wordMaxSize){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addLexeme(words, tmp, &i, &j, wordMaxSize, *classes, &m, classMaxSize, wordCount);
                    j = 0;
                }
                if(j > wordMaxSize - 1){
                    tmp = doubleWordSize(tmp, wordMaxSize);
                    wordMaxSize *= 2;
                }
                if(j < wordMaxSize){
                    tmp[j] = c;
                    j++;
                }
                while((c = fgetc(fp)) != '\n'){
                    if(j > wordMaxSize - 1){
                        tmp = doubleWordSize(tmp, wordMaxSize);
                        wordMaxSize *= 2;
                    }
                    if(j < wordMaxSize){
                        tmp[j] = c;
                        j++;
                    }
                }
                addWord(words, tmp, &i, &j, wordMaxSize);
                addClass(*classes, COMMENT, &m, classMaxSize);
                (*wordCount)++;
                continue;
            }
            if(c == '"'){
                if(j > 0){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addLexeme(words, tmp, &i, &j, wordMaxSize, *classes, &m, classMaxSize, wordCount);
                    j = 0;
                }
                if(j > wordMaxSize - 1){
                    tmp = doubleWordSize(tmp, wordMaxSize);
                    wordMaxSize *= 2;
                }
                if(j < wordMaxSize){
                    tmp[j] = c;
                    j++;
                }
                while((c = fgetc(fp)) != '"'){
                    if(j > wordMaxSize - 1){
                        tmp = doubleWordSize(tmp, wordMaxSize);
                        wordMaxSize *= 2;
                    }
                    if(j < wordMaxSize){
                        tmp[j] = c;
                        j++;
                    }
                }
                if(j > wordMaxSize - 1){
                    tmp = doubleWordSize(tmp, wordMaxSize);
                    wordMaxSize *= 2;
                }
                if(j < wordMaxSize){
                    tmp[j] = c;
                    j++;
                }
                if(j > 0){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addWord(words, tmp, &i, &j, wordMaxSize);
                    addClass(*classes, CONSTANT, &m, classMaxSize);
                    (*wordCount)++;
                    continue;
                }
            }
            if(isSeparator){
                if(j > 0){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addLexeme(words, tmp, &i, &j, wordMaxSize, *classes, &m, classMaxSize, wordCount);
                }else{
                    j = 0;
                }
            } else if(isPunctuator){
                if(j > 0){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addLexeme(words, tmp, &i, &j, wordMaxSize, *classes, &m, classMaxSize, wordCount);
                }
                tmp[j] = c;
                j++;
                if(j > 0){
                    if(i > wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        *classes = doubleArraySize(*classes, wordArraySize);
                        classArraySize *= 2;
                        wordArraySize *= 2;
                    }
                    addWord(words, tmp, &i, &j, wordMaxSize);
                    addClass(*classes, PUNCTUATION_SIGN, &m, classMaxSize);
                    (*wordCount)++;
                }
            }else{
                if(j > wordMaxSize - 1){
                    tmp = doubleWordSize(tmp, wordMaxSize);
                    wordMaxSize *= 2;
                }
                if(j < wordMaxSize){
                    tmp[j] = c;
                }
                j++;
            }
        }
        if(j > 0 && j <= wordMaxSize){
            if(i > wordArraySize - 1){
                words = doubleArraySize(words, wordArraySize);
                *classes = doubleArraySize(*classes, wordArraySize);
                classArraySize *= 2;
                wordArraySize *= 2;
            }
            addLexeme(words, tmp, &i, &j, wordMaxSize, *classes, &m, classMaxSize, wordCount);
        }
        
        fclose(fp);
    }
    *wordCount = i;
    return words;
}

int main(int argc, const char * argv[]) {

    int word_array_size = 16;
    int class_array_size = 16;

    
    char **words = (char**)malloc(word_array_size * sizeof(char*));
    char **classes = (char**)malloc(class_array_size * sizeof(char*));
    int wordCount = 0;
    char *filename = DEFAULT_INPUT_FILE_NAME;
    char ***pclasses = &classes;
    FILE *fp = fopen(filename, "r");
    words = parseWordsFromFile(words, pclasses, fp, WORD_MAX_SIZE, word_array_size, CLASS_MAX_SIZE, class_array_size, &wordCount);
    int wordsCountArr[wordCount];
    for (int i = 0; i < wordCount; i++) {
        wordsCountArr[i] = 0;
    }
    fclose(fp);
    
    for (int i = 0; i < wordCount; i++) {
        printf("<%s, %s>\n", words[i], classes[i]);
    }
    for (int i = 0; i < wordCount; i++) {
        if(words[i] != NULL){
            free(words[i]);
        }
    }
    free(words);
    free(classes);
    
    return 0;
}
