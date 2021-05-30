# Industrial Visit Planning & Booking System (Community part)

## 1. Use cases
저희 프로젝트의 Community 파트는 총 5가지의 **Use case** 로 구분됩니다. 


각 Use case에 대한 간략한 설명을 하자면 
아래 기술된 사항들과 같습니다.

### Use case 1 : 커뮤니티 메인메뉴 디스플레이
***

커뮤니티 페이지의 방문자가 메인메뉴를 방문할 시 

후기, 준비, 자유 게시판 각각에서 조회수 기준 상위 3개의
게시글에 대한 정보를 볼 수 있습니다. 

### Use case 2 : 커뮤니티 게시물 검색
***

방문자는 커뮤니티의 메인 페이지에서 검색창에 키워드를 입력하여 

해당 키워드 내용을 제목으로 포함하고 있는
게시글들의 목록을 볼 수 있습니다.


###  Use case 3 : 커뮤니티 게시물 관리
***

1. 방문자는 조회하고자 하는 게시물을 클릭하여 해당 게시물의 제목, 내용, 댓글, 조회수 등을 확인할 수 있습니다.


2. 방문자가 메인 페이지에서 글 작성하기 버튼을 누른 후 글을 작성하면 해당 게시글의 카테고리에 해당하는 게시판에 작성한 게시글이 나타납니다.


3. 마이페이지에서 방문자는 본인이 작성한 수정하고자 하는 게시글을 선택하여 수정할 수 있습니다.


4. 마이페이지에서 방문자는 삭제하고자 하는 게시글을 선택하여 삭제할 수 있습니다.




###  Use case 4 : 커뮤니티 게시물 댓글 관리
***

방문자는 커뮤니티의 메인 페이지에서 검색창에 키워드를 입력하여 해당 키워드 내용을 제목으로 포함하고 있는
게시글들의 목록을 볼 수 있습니다.


###  Use case 5 : 산업체 패키지 후기 및 댓글 관리
***

1. 방문자는 특정 산업체 방문 예약 패키지 하단에 후기 작성하기 버튼을 클릭하여, 방문했던 산업체 패키지에 대한 후기를 남길 수 있습니다.


3. 방문자는 특정 게시글에 대하여 댓글을 작성할 수 있습니다.


5. 방문자는 마이 페이지에서 본인이 작성한 후기 혹은 특정 게시글에 대한 댓글을 수정하거나 삭제할 수 있습니다.


***
## 2. Domain Models

프로젝트에 사용된 도메인 모델에 대해서는 간단하게 어떠한 responsibility들 있는지 설명하겠습니다.

***
### Controller
```
Type은 D(data), C(Controller)이며 커뮤니티 페이지의 모든 activity들을 관리합니다.
```
### InputEntry
```
Type은 D(data), B(Boundary)이며 사용자가 입력한 값을 전달하는 데 사용됩니다.
```
### ServiceDisplay
```
Type은 D(data), B(Boundary)이며 웹페이지를 디스플레이 하는 역할을 합니다.
```
### UserInput
```
Type은 K(know), B(Boundary)이며 사용자의 입력 값을 저장합니다.
```
### DBOperator
```
Type은 D(Data), B(Boundary)이며 쿼리문을 받아 해당 쿼리문을 처리해서 결과를 받아오거나 그 결과를 DB에

입력합니다.
```
***
### PostProcessor
***
```
Type은 D(Data), C(Controller)이며 자주 사용될 쿼리문의 형식을 저장해 두었다가, DBOperator에 사용될
쿼리로 넘겨주는 역할을 합니다.
```

### PersonalInfoDisplay
***
```
Type은 K(know), B(Boundary)이며 유저에 대한 개인 정보를 디스플레이합니다.
```

### ReportUser
***
```
Type은 D(data), B(Boundary)이며 커뮤니티 정책을 위반하는 게시글 및 댓글을 신고합니다.
```

### RedirectUser
***
```
Type은 D(Data), B(Boundary)이며 사용자가 웹 페이지에서 버튼을 클릭하거나 웹 페이지에
url을 바꾸었을 때 이에 해당하는 페이지로 이동할 수 있도록 처리합니다.
```
***

## 3. Sequence Diagram
sequence diagram은 Use case 1,2를 제외하고 각 Use case별로 작성되었습니다.
각 use case별 sequence Diagram은 다음과 같습니다.
### Use Case 1,2
***
![1](https://user-images.githubusercontent.com/11494592/120093668-b8ff0100-c156-11eb-9581-6051aae185eb.PNG)
![2](https://user-images.githubusercontent.com/11494592/120093669-ba302e00-c156-11eb-9e4b-de7e038cde00.PNG)
![3](https://user-images.githubusercontent.com/11494592/120093675-c0bea580-c156-11eb-8814-273140de77a1.PNG)
위 sequence diagram은 유저가 메인 페이지에 들어왔을 때 각 게시판 별 조회수 기준 상위 3개 게시글을 디스플레이 하는 기능과
유저가 검색창에 검색 키워드를 입력했을 때 그 키워드에 해당하는 게시글들을 디스플레이 하도록 하는 기능,
그리고 메인 페이지에서 후기, 준비, 자유 라는 버튼을 클릭했을 때 클릭한 카테고리에 해당하는 게시글들을 디스플레이 하는 기능을
설명한 sequence diagram입니다.
### Use Case 3
***

![use case 3](https://user-images.githubusercontent.com/11494592/120093676-c4522c80-c156-11eb-93dd-d344f7509d6f.png)
유저가 게시글을 삭제, 수정, 작성할 때 발생하는 과정을 표현한 sequence diagram입니다.
### Use Case 4
***
![4](https://user-images.githubusercontent.com/11494592/120093681-cb793a80-c156-11eb-8d0d-e53080ebf4c2.PNG)
![5](https://user-images.githubusercontent.com/11494592/120093682-cc11d100-c156-11eb-970b-7f1aa7bf7cc9.PNG)
![6](https://user-images.githubusercontent.com/11494592/120093684-cd42fe00-c156-11eb-90ba-a75f2eb5e9b1.PNG)


게시물에 대한 댓글 작성, 삭제, 수정 그리고 커뮤니티 정책을 위반하는 댓글을 신고하는 기능을 표현한 sequence diagram입니다.

### Use Case 5
![use case 5-1](https://user-images.githubusercontent.com/11494592/120093898-295a5200-c158-11eb-81cb-44b6bdfede4f.png)
![use case 5-2](https://user-images.githubusercontent.com/11494592/120093686-cf0cc180-c156-11eb-9dfe-35c73f9909b8.png)
산업체 패키지 후기 및 그 후기에 해당하는 댓글에 대한 작성, 편집, 삭제 기능을 나타낸


sequence diagram입니다.
### 테스트는 이런 식으로 작성하시면 됩니다

```
예시
```

## 3. Sequnece Diagram

Add additional notes about how to deploy this on a live system / 라이브 시스템을 배포하는 방법

## Built With / 누구랑 만들었나요?

* [이름](링크) - 무엇 무엇을 했어요
* [Name](Link) - Create README.md

## Contributiong / 기여

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us. / [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) 를 읽고 이에 맞추어 pull request 를 해주세요.

## License / 라이센스

This project is licensed under the MIT License - see the [LICENSE.md](https://gist.github.com/PurpleBooth/LICENSE.md) file for details / 이 프로젝트는 MIT 라이센스로 라이센스가 부여되어 있습니다. 자세한 내용은 LICENSE.md 파일을 참고하세요.

## Acknowledgments / 감사의 말

* Hat tip to anyone whose code was used / 코드를 사용한 모든 사용자들에게 팁
* Inspiration / 영감
* etc / 기타
