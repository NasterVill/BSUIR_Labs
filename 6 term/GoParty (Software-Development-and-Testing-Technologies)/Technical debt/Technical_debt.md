# Technical debt

### Overview

After code examining we came to a decision, that the architecture solutions, both for client and back end applications, were chosen right. Both projects proved themselves to be relatively expandable and flexible, despite some sacrifices made to ease the development process. No duplicate code parts were found.

A list of problems which we have faced consists of:

* Low test coverage;
* Mobile client and back-end are not fully connected because first one is behind the second one;
* Back end application is not deployed;
* UI lacks quality;
* No automated testing is set up;

### Technical debt description
* Both applications haven't got a necessary test coverage and all the test are manual -- not automated. That brings possible difficulties with continiuos integration, because of inability for quick and asured testing.

* Mobile client lacks the functionality of event's subscription and other users' profiles view. This is a major debt, because it is directly connected to the most vital core functionality of the application, which is being able to find new friends, through subscriptions to different events and viewing their profiles.

* UI quality can be improved. There is plenty of room for UI changes, for example:  register screen can be divided into multiple screens in order to decrease the amount of fields, which are required to be filled with data. Also the event list should be refactored, so that user would scroll the endless list instead of surfing through pages.

### List of unimplemented features

* Event subscription and side user profile view
* Tags
* Moderator functionality

### Reasons 'why' and ways of solving the problems
* Low level of coverage can be explained in the next way: first of all team's goal was to implement as much raw functionality as it is possible. That is why the code is not yet covered with tests as it should be. To make the situation better we plan to create tickets for both client and back end applications about test coverage expansion.

* The application is not deployed because there is not a bunch of services to host it. We decided to use [Smart ASP.NET](https://www.smarterasp.net/index) to solve this issue. Still this requires great efforts from back end development team.

* Project has features that are not yet fully ready, because other features, that came before, took more effort to implement. The remaining functionality is not so complicated, but due to mistakes in earlier stages of application development this features remain undone. Tags, subscriptions and side user profiles are done on back end side, while client is not yet ready to work with them. To ensure debt's closure tickets will be created respectively for each yet unimplemented feature.

* The lack of UI quality may be justified by the fact, that our team didn't have much design experience. It can be considered as moderate, but requires further improvements. To improve the UX we plan to produce a list of those UI components that should be changed and then create tickets respectively.
 
### Tickets to close the debt
* [Event subscribtion](https://trello.com/c/k98Kd2d1/63-feature-mobile-goparty23subcribeimplementation)
* [Tags](https://trello.com/c/07IxRDfs/64-feature-mobile-goparty24tagimplenetation)
* [Moderators](https://trello.com/c/Opo7yTFC/65-story-8-as-a-moderator-i-want-to-be-able-to-accept-decline-the-creation-of-users-events)
