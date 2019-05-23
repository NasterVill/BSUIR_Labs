# Architechture solution research

### 1. Application type
  GoParty is a `Mobile Application`. This type was chosen because nowadays smartphones are getting all over the place and are being integrated in our lives deeply. This type provides availability and simplicity of usage.
The application itself is supposed to:
* Have a minimalistic user-friendly interface
* Use internet resources wisely and be able to sustain its work with flaky internet connection
* Optimize the amount of used storage and processing resources

### 2. Deploying strategy

  GoParty application has a `distributed` `client-server` deployment model. The back end of the application is storing and processing data on one server, so that the database is not separated from the represenation model. This solution makes the architechture less flexible but provides greater performance and  less code complexity.

### 3. Technologies description

| Objective | Technology (and/or Language) | 
|:---|:---|
|Mobile client|Kotlin|
|Server application|.NET Framework|
|Web server|Internet Information Services (IIS)|
|Database configuration|SQL Server Managment Studio|
|Database server|MS SQL Server|
|IDE|MS Visual Studio, Android Studio|

1) For mobile client development Kotlin was chosen as a primary language, because of its high popularity, good support from Google and its performance (in context of Android-based systems)
2) .NET helps to develop high quality applications faster. Modern language constructs like generics, Language Integrated Query (LINQ), and asynchronous programming make developers productive. The ASP.NET Web API is an extensible framework for building HTTP based services that can be accessed in different applications on different platforms such as web, windows, mobile etc.
3)  With the Entity Framework, developers can work at a higher level of abstraction when they deal with data, and can create and maintain data-oriented applications with less code compared with traditional applications.

### 4. Quality markers
Here are listed all the important  quality markers

| Category |  Quality marker| 
|:---|:---|
|Desing quality|Conceptual integrity|
|Runtime qualuty|Availabilty|
||Performance|
||Reliablity|
||Security|
|User interface quality|Convinience and usage simplicity|

### 5. Ways to implement end-to-end functionality
* Authentication is based on the OAuth2 protocol that contains encrypted user information.
* Network communication between client and server occurs via Http requests.
* The configuration of the application is stored on the server as an xml file.
* Exception management with exception handlers which are included in the framework pipeline.
### 6. Application structure
![structure](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Architecture/Structure.PNG)
### 7. Arcitecture 'As Is' and 'To be'

## Back end
![arcitecture_back_as_is](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Architecture/Architecture(Back%20end).PNG)

  The stair architecture (Stairway pattern) is a variation of the classical three-layer architecture with the Dependency Inversion Principle. As a result of this pattern, the system becomes more resistant to change. If you change the implementation of each level, you do not have to change the rest, if the interaction between them remains unchanged. This flexibility is achieved both by providing strong cohension (high cohension) and weak coupling (low coupling) components of the application.
  
## Comparison:
  In general architectural idea was successfully implemented. But there are some issues inside of concrete layers. All of this differences are trivial and temporarily that's why it's not shown on another diagram. 
  
## Ways of improvement:
  1. Move mappers to separated interface 
  2. Improve exception handling mechanism
  3. Utilize DependencyResolver component due to using NinjectModule implementation for each binding context.
  
## Mobile client

### As to be

CleanArchitecture

![arcitecture_mobile_to_be](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Architecture/Clean%20Architecture%20(Front%20end).jpg)

Clean architecture pattern was initially chosen because of its high flexibility and expandability. Usually the application is divided into 4 layers: 
* Presentation
* Use cases
* Domain
* Data
* Framework

First level serves to present the information to the user, second one is interacting with him (through touches, clicks and so on), third one is responsible for business logic handling, the fourth one is representing a layer for data management  and the last one is used to fetch or send data from/to some sort of framework.

### As it is

![arcitecture_mobile_is](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Architecture/Architecture(Front%20end).PNG)

While developing the application, a decision was made, to combine first and second, third and fourth layers respectively in order to ease the overall development. In this "modified" architecture the first layer (User Interface) is responsible for both presenting the inforamtion and interacting with the user, the second one (Business logic) handles data processing and holds application's core logic, the third layer (Data layer) is responsible for the communication with the back end application.

## Comparison:

The architecture solution that was chosen for mobile client can be considred as a "light" clean architecture, with the interpretation, which provides an ability for easing the development and support because of lesser complexity, but, in future, this may bring some issues with logic refactoring if one will be required. Our team decided to go with this solution, because we thought, that the issues with business logic should be handeled on the earlier levels of project development, long before the actual code writing begins

## Class diagrams

* [Monile client](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Class/Mobile_Client_Diagram.png)
* [Back end](https://github.com/NasterVill/GoParty/blob/master/Resources/Diagrams/Class/Back_end_Diagram.PNG)
