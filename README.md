# DropoutToDoughlord
<p align="center">
  <img width="512" height="463" alt="splash1" src="https://github.com/user-attachments/assets/9cacd8fa-f59a-49be-9ed9-2b6efcb1c515" /><br>
  <em>Figure 1. The Hire and Fire Screen at a particular location</em>
</p>
<br>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dropout to Doughlord: Pizza Tycoon is a not quite finished, text-based tycoon like game, where the user, operates a restaurant in an attempt to make profit, allowing for upgrades to equipment, hiring of employees, and expanding to new locations(over 180). You begin the game as a disgruntled student, fleeing the scene after you bombed you Advanced C++ final, and end up taking over Pizza Fiesta right across the street.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;After deciding on the name for your pizza restaurant, you decide on the prices for your three dishes offered on the menu. You sell a single slice of pizza, and eight cut pizza, and a ‘twelve cut with wings’ combo. This takes to one of the most important menus, the action menu, here is where the user can decide on taking a variety of actions, including viewing the currently owned locations, proceeding to next week, and viewing various reports that really only become interesting after opening a few more locations.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The player starts $1444.11 leftover in balance after renovations, and you currently rent the Pittsburgh location for $2800 a week. The main game loop, is to ‘proceed to next week’ to make some profit, and then invest in your locations to be able to increase your earning potential. Your earnings are calculated by multiplying your set prices(can be changed later) by the number of sales you make. Every location has a defined base demand for each of the three menu options, acting as the ‘potential number of sales’. This however tends to decrease after factoring in the price. Every location also has a ‘wealth’ attribute, determining how price sensitive the locals are. Pittsburgh is ‘Middling’, which means setting the price too high above base price will drastically limit the amount of customers. 
<br>

<p align="center">
  <img width="512" height="260" alt="wealth3" src="https://github.com/user-attachments/assets/cc8495ed-9b59-40e9-b4e4-62fae84e5b8e" /><br>
  <em>Figure 2. Graph of Purchase probability in game as a function of %-difference to base price. </em>
</p>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;You’ll notice the different equations for determining the number of sales we actually make. We start with the base sales, largely impacted by base demand, some employees (more on that later) and a hint of variance, which differs slightly from location to location. We then determine a purchase probability from the functions above. We mentioned how Pittsburgh is a ‘Middling’ location, meaning that if you set your prices to 5x base prices, or $15.00 for a single slice, you’ll only get about 20% of the estimated single slice demand, or 173 sales. The estimated demand for any location you own is viewable from the location screen.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;You might notice that given the graph of purchase probability above, that wealthy locations are very price ‘inelastic’. This is a design flaw and subject to change in future versions. However, if you are having trouble making money, (can be tough!) open a shop in a wealthy location and set your prices as high as you would like!<br><br>
<div align="center">
  <table>
  <tr>
    <td>Cook</td>
    <td>handles up to 180 orders per week</td>
  </tr>
  <tr>
    <td>Cashier</td>
    <td>handles up to 750 orders per week</td>
  </tr>
  <tr>
    <td>Dishwasher</td>
    <td>handles up to 1650 orders per week</td>
  </tr>
</table>
  <em>Figure 3. Chart of Employees and their bonuses</em>
</div>
<br>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This brings us to the staff that we can hire at each location, allowing us to operate. We have six different types of employees, described below.Cooks, Cashiers and Dishwashers are the backbone of our restaurants, and each of them can be a limiting factor every week. For example if we had one of each of them hired, we could only make 180 orders for the week, as Cooks would be the bottleneck. 

<div align="center">
  <table>
  <tr>
    <td>Delivery Driver</td>
    <td>+20 Base Demand for all Menu items, Max of 2</td>
  </tr>
  <tr>
    <td>Manager</td>
    <td>Cooks can make 20 extra orders per week each, they also automatically purchase supplies, provided you have enough money. Max of 2</td>
  </tr>
  <tr>
    <td>General Manager</td>
    <td>Cooks, Cashiers, and Dishwashers can handle +50 orders per week each. They also provide a 7.5% discount to the Cost of Goods Sold, and most importantly, will automatically hire employees if needed. Max of 1</td>
  </tr>
</table>
  <em>Figure 4. More employees, more bonuses</em>
</div>
<br>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;How many employees you need to operate differs from location to location, along with their pay. In Pittsburgh for example, dishwashers make $10 per hour, cooks make $12, cashiers $9, delivery Guys $4, managers $16, and general managers $25. In general poorer locations tend to pay significantly less. For example, in Lagos Nigeria, general managers make $11 per hour. All employees work 40 hours per week, and their weekly pay is listed along with their hourly in the ‘Manage Employees’ submenu of a location. The employees are an integral part, but not the only part. You’ll notice that managers and general managers impact supplies. What are supplies?
To make orders, you’ll also need ingredients, namely dough, sauce, cheese, and for the 12 cut combo, wings. Each of our menu items requires a different amount of these to make. A single slice of pizza for example, costs 0.125 dough, sauce and cheese, and as you might imagine, an ‘8 Cut’ costs 1.00 of each. It’s important to note, that ingredient costs to make an order DO NOT change from locations. A single slice will always cost 0.125 of each of the three ingredients, However the cost of these ingredients does change from location to location.
<br><br>
<div align="center">
  <img width="512" height="121" alt="labor4" src="https://github.com/user-attachments/assets/f785870e-9466-4b9d-a510-846bf30985cd" /><br>
  <em>Figure 5. Chart of Supplies, their cost, and how many remain in stockpile in Pittsburgh. Notice the -7.5% Discount from having a GM</em>
</div>
<br>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;One last major impact to consider is the equipment we have in a location. There are currently 4 types of equipment in the game, although the ‘robot’ is currently pointless. The others being the Pizza Oven, Fryer and Fridge. Similar to our employees, the Pizza Oven and Fryer act as limits to the amount of orders we can make per week, but can be upgraded for a small fortune with a large impact. We start with a level 1 of all three in Pittsburgh and when opening new stores, translating to a limit of 1250 orders with the pizza oven, and 60 orders of wings per week. It’s also worth noting, that all equipment has a maintenance cost that varies across locations, and scales linearly with the level, however it’s usually a very low cost for the business.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The fridge is slightly different as it doesn’t impact our sales directly, it merely determines the maximum amount of supplies we can hold in our fridge for an entire week. With a level 1 fridge, we can store up 1400 of each of the ingredients, counted separately, and every additional level increases by 1400.<br>


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;As our business grows from our humble beginnings in the ruins of Pizza Fiesta, to an international juggernaut with dozens of locations scattered throughout the world, you might find that buying supplies every week becomes tiring, and you might ask yourself? Can I hire someone who can help me? This is where managers come in! Given you have at least $100,000 in your balance at the end of the ‘simulate until next week’ they will automatically purchase supplies for the relative cost in their location. Note that if your balance dips below $100,000 during the simulation, supplies are purchased in the order locations were opened (Pittsburgh is first). <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This also applies to another burden lifted off the shoulders of our CEO, as general managers, provided you have $200,000 will automatically hire employees and upgrade equipment if they become a bottleneck, and provided you have enough money. Funny enough, if you begin to run out of supplies, a general manager will actually hire a manager to buy supplies for you! I personally have found myself only hiring a general manager upon opening a new location, and within a couple of weeks they will sort it our for you! You might be skeptical of high wage cost for management, but as you begin to have even just a few locations, they become worth their weight in gold!<br>

### Weekly Report
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;As touched on briefly, once you are finished hiring employees, upgrading equipment and adjusting your prices, you can ‘proceed to next week’ which simulates your business over the course of the entire week. It determines the amount you’ll do in sales, calculates your revenue, uses up the appropriate amount of supplies, pays employees and more. After simulating you will be shown a weekly report for the company, detailing every location’s vital information in a condensed format. We can see our revenue, Cost of Goods Sold, payroll and maintenance costs, and if the manager bought supplies. 
<br>
<div align="center">
  <img width="512" height="268" alt="balance5" src="https://github.com/user-attachments/assets/3c242450-5d4e-4cdf-94e1-59051c705eb2" /><br>
  <em>Figure 6. A snippet from the weekly report, detailling revenue and costs per location</em>
</div>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;At the bottom of the weekly report is a summary providing one line for every location we own, out net revenue, and the factors that made it profitable. You’ll notice that the cost of rent is listed in locations where we didn’t buy, as well as our profit. We also must pay taxes, (only on profit) in every location, leading to the sum of our net revenue. We also have managers in all of our locations whom bought supplies, hence the $32,452.41 charge for supplies. However, even if we had no managers, we would still have a calculated and listed Cost of Goods Sold, however, we would have to buy supplies for ourself.<br>


<br>
<div align="center">
  <img width="2319" height="700" alt="table6" src="https://github.com/user-attachments/assets/bcb0add2-28f0-414d-affd-19378d9273cb" /><br>
  <em>Figure 7. Another weekly report, after we have opened a few locations. We pay more than $40,000 per week in taxes!</em>
</div>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This is the main game loop of the game, we slowly expand our business, hire employees, buy supplies, upgrade equipment, and change our prices. Currently there are 181 Locations in the game, spread across nine regions, and multiple dozen countries. You begin the game barely able to keep your head above water paying for a few employees, and within a few years paying more in taxes than you made in your whole first year! Old Man Rick would be so proud!

### Appendix
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The various reports are available at any time in the main options screen. In future versions, it is planned that these reports are unlocked by hiring various corporate employees whom 'write' these reports amongst other bonuses.
<br>
<div align="center">
<img width="2082" height="492" alt="headcount" src="https://github.com/user-attachments/assets/7aa20ff3-c8d3-4332-8c32-eb46f6bb0f50" /><br>
  <em>Appendix 1. The headcount report</em>
</div>
<br>

<br>
<div align="center">
<img width="1909" height="461" alt="COGS" src="https://github.com/user-attachments/assets/582300b0-e98d-4f8b-9bd2-7576418a5ec0" /><br>
  <em>Appendix 2. The 'Cost of Goods Sold' report</em>
</div>
<br>

<br>
<div align="center">
  <img width="2012" height="514" alt="INVENTORY" src="https://github.com/user-attachments/assets/a6f214f9-361f-4363-bfd0-8fbd570e6ddc" /><br>
  <em>Appendix 3. The inventory report</em>
</div>
<br>
