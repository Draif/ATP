Павел Яковенко - поисковик.

***Иструкция по сборке***
Можно просто запустить make из директорией с программой. Внутри также запускается flex и bison(подробнее о них ниже), но вроде они идут в стандартом пакете для linux.

***Использование***
На вход программе подается путь до книги или текста. Дальше программа индексирует текст и приглашает пользователя вводить запросы. Каждый запрос - несколько слов.
После нажатия Enter начинается поиск предложений. Сначала показываются первые 10 наиболее релевантных предложения, так сказать первая страница поисковой выдачи. Далее
пользователь может ввести команду "n" - чтобы показать следующую страницу выдачи, "all" - чтобы вывести сразу оставшую часть поисковой выдачи, или "s" - чтобы больше не показывать
результаты поиска и сразу перейти к следующему запросу. Завершается программа посылом сигнала программе "EOF" (обычно Ctrl - D). 
Программа  гарантированно работает на двух книгах, приложенных с программой - "harry-potter-and-the-sourcerer-stone.txt" и "All_Quiet_on_the_Western_Front".

***Архитектура***
Мой поисковик состоит из нескольких частей - парсера текста, бора со словами и индексации, а также самой поисковой машины.
Далее я опишу все части. В моей программе использованы несколько стороних библиотек и утилит, но все они являются проектами со свободным исходным кодом. Также я использовал boost. У меня стоит 
версия 1.55, но так как я использовал только boost::shared_ptr<>, более ранние версии его тоже должны поддерживать. В моей программе также активно используется паттерн singleton, так
как многие объекты классов должны быть только в единственном числе и они должны быть доступны из разных частей программы. 

    Парсер текста
        Мой парсер состоит из лексера и синтаксического анализатора. Лексер создан с помощью flex (подробнее http://flex.sourceforge.net/, http://en.wikipedia.org/wiki/Flex_lexical_analyser).
        Flex scanner находиться в файле "lexer.l". Файл "lex.yy.c" создается при выполнение команды "flex lexer.l". Далее распознаные леммы передаются в синтаксический анализатор, созданный при
        помощи Bison (подробнее http://www.gnu.org/software/bison/, http://en.wikipedia.org/wiki/GNU_bison). Сама грамматика описана в "grammatics.y". "grammatics.tab.c" и "grammatics.tab.h"
        создаются автоматически после запуска bison.
        Из лексера и анализатора распознаные слова и cимволы подаюстся в класс, находящийся в  "TextParser.h". Оттуда они направляются в бор из слов.
    
    Бор слов и индексация
        Бор слов лежит в файле "WordsBor.h". Это просто обычный бор, где ребра - это символы. Вершины бора - LetterNode. Этот класс находится в том же файле. Каждая такая нода может быть концом 
        какого то слова - тогда выставлен флаг isWord, или быть просто промежуточной вершиной. Внутри каждой ноды также храниться IDF слова. Класс Indexer - просто мномежуточный класс
        связывающий TextParser и WordsBor. Когда добавляется новое слово, происходит его стемминг. Стеммер лежит в файле "Stem.h". Он реализован с помощью билиотеки Snowball 
        (http://snowball.tartarus.org/index.php). Стеммерное слово тоже добавляется в бор слов.

    Поисковая Машина
        После набора запроса вызывается простой парсер запроса - описан в "main.cpp". Он парсит слова, и отправляет их в Searcher ("Searcher.h"). Там анализируется запрос и выводится поисковая
        выдача. Также в запрос добавляются стеммерные слова. Если введенного слова из запроса нет в тексте, он попытается найти наиболее близкое к нему слово.
        Поисковое ранжирование у меня примерно такое  - для каждого слова берутся номера документов ( иначе предложений), где оно встречается. Для каждого документа считается
        количество слов из запроса, которые в нем встречаются, а также его вес для конктретного слова. Веса считаются с помощью  функции ранжирования Okapi BM25 (http://en.wikipedia.org/wiki/Okapi_BM25). 
        Потом веса суммируются для каждого документа. Документ с наибольшем количеством слов из запроса идет в топ. Если у нескольких документов одинако количество слов из запроса в нем,
        то они ражируются с помощью  okapi. Далее все документы сортируются в соответсвие с правилом, изложенном выше и выдаются пользователю по 10 документов на странице. 



