# TeamSpeak 3 Badge Spoofing

<b>This repository will be archived as TeamSpeak fixed this method in their 3.3.0 Release.</b><br> Badge spoofing does not work anymore.<br>
https://forum.teamspeak.com/threads/140245-Release-TeamSpeak-Client-3-3-0<br>
> In previous versions of our client and/or server, it was possible to inject fake badge data. We've eliminated this problem with a new feature called Signed-Badges. If a user configures a badge in his/her myTeamSpeak account settings, the server will now validate this info and only show the badge icon if the user really possesses it.

Thanks to Suphax for letting me know.<br><br>
<hr>

TeamSpeak 3 introduced badges a while ago. Badges are small icons next to your name, you can get them if you redeem the proper code for it on your MyTeamSpeak account.<br>
<img src="https://i.imgur.com/IdmUPod.png"></img><br>
Those codes may e.g. be distributed on some gaming related events, such as the E3 or Gamescom, or are exclusively given to TeamSpeak plugin authors.
Since these codes are sometimes very limited and exclusive, it is not easy to get the ones you desire.

There is a set of badge ids that can be sent to your current teamspeak server, and no further instance will check if you actually own these badges on your MyTeamSpeak account. This allows you to spoof your badges on any teamspeak server and impress your friends.


This is the current list of badge ids:

    1cb07348-34a4-4741-b50f-c41e584370f7 - Creator of TeamSpeak Addons
    50bbdbc8-0f2a-46eb-9808-602225b49627 - Registered during Gamescom 2016
    d95f9901-c42d-4bac-8849-7164fd9e2310 - Registered during Paris Games Week 2016
    62444179-0d99-42ba-a45c-c6b1557d079a - Registered at Gamescom 2014
    d95f9901-c42d-4bac-8849-7164fd9e2310 - Registered at Paris Games Week 2014
    450f81c1-ab41-4211-a338-222fa94ed157 - Creator of at least 1 TeamSpeak Addon
    c9e97536-5a2d-4c8e-a135-af404587a472 - Creator of at least 3 TeamSpeak Addons
    94ec66de-5940-4e38-b002-970df0cf6c94 - Creator of at least 5 TeamSpeak Addons
    534c9582-ab02-4267-aec6-2d94361daa2a - Visited TeamSpeak at Gamescom 2017
    7d9fa2b1-b6fa-47ad-9838-c239a4ddd116 - MIFCOM | Entered Performance
    34dbfa8f-bd27-494c-aa08-a312fc0bb240 - Gaming Hero at Gamescom 2017
    f81ad44d-e931-47d1-a3ef-5fd160217cf8 - 4Netplayers customer
    f22c22f1-8e2d-4d99-8de9-f352dc26ac5b - Rocket Beans TV Community
    64221fd1-706c-4bb2-ba55-996c39effa79 - MyTeamSpeak early adopter
    c3f823eb-5d5c-40f9-9dbd-3437d59a539d - New MyTeamSpeak member
    935e5a2a-954a-44ca-aa7a-55c79285b601 - Discovered at E3 2018
    4eef1ecf-a0ea-423d-bfd0-496543a00305 - Visited TeamSpeak at Gamescom 2018
    24512806-f886-4440-b579-9e26e4219ef6 - Played at Gamescom 2018
    b9c7d6ad-5b99-40fb-988c-1d02ab6cc130 - Met Tim Speak
Source: (raw) http://badges-content.teamspeak.com/list, (with images) http://yat.qa/ressourcen/abzeichen-badges/

<br><br>
I made this tool to be able to permanently wear my favorite badges. This project compiles as a TeamSpeak plugin (dll), which needs to be moved to the folder at %appdata%\TS3Client\plugins.

The badges can be configured in the <a href="https://github.com/A200K/TeamSpeak3_Badge_Spoofing/blob/master/TeamSpeak%203%20Badge%20Editor/Main.cpp#L106">Main.cpp</a> file:
	
    // Creator of at least 5 TeamSpeak Addons, Registered at Gamescom 2014, Discovered at E3 2018
    SendCommandToServer( "clientupdate client_badges=Overwolf=0:badges=94ec66de-5940-4e38-b002-970df0cf6c94,62444179-0d99-42ba-a45c-c6b1557d079a,935e5a2a-954a-44ca-aa7a-55c79285b601" );

This command will be executed as soon as you connect to a teamspeak server and it sets your selected badge ids.

Before:<br>
<img src="https://i.imgur.com/VK95lrB.png"></img>

After:<br>
<img src="https://i.imgur.com/PBriUXn.png"></img>

<b>Please do note that you won't be able to see the badges yourself, but anyone else will see them.</b> 
You can see them by connecting to your currently connected server on a second tab on teamspeak.

Since you need to hardcode the badge ids (and thus compile the project yourself) at the moment, there is no binary release for this project so far. I plan to add a GUI to easily set the badges in the future.

