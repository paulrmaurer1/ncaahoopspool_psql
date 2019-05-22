<!--block of code to display the Roster table-->
<?php
    /*Display Delete Button for admin user*/
    if($_SESSION['logname']=='admin')
    {
	echo "<p class=\"submit\" style=\"margin-left:550px\"><input type=\"submit\" name=\"display_button\" value=\"Delete Player(s)\"/></p>";
    }
?>
<table class="roster">
    <thead>
        <tr>
            <th></th>
            <th class="name">Name</th>
            <th class="email">Email</th>
            <th class="team">Team</th>
            <th class="wins"># Wins</th>
            <?php if($_SESSION['logname']=='admin') {echo "<th class=\"action\">Delete</th>";} ?>
        </tr>
    </thead>
    <tbody>
        <?php
        /*Display roster table for all players (except don't list admin)*/
	$counter=0;
	$query = "SELECT * FROM player";
	$result = pg_exec($cxn, $query) or die ("Couldn't execute player roster query.");
	while ($row=pg_fetch_assoc($result))
	{		
                extract($row);
		if ($email<>'admin')
		{
		    $counter=$counter+1;
		    echo "<tr>\n";
		    echo "<td>$counter.&nbsp</td>\n";
		    echo "<td>$firstname"." "."$lastname</td>\n";
		    echo "<td>$email</td>\n";
		    $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
		    $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
		    $trow=pg_fetch_assoc($tresult);
		    extract($trow);
		    echo "<td>$team_name</td>\n";
		    echo "<td>$num_wins</td>\n";
		    if($_SESSION['logname']=='admin') /*only include checkboxes for admin user to select players for deletion*/
		    {
			echo "<td><input type='checkbox' name='deleteplayer[]' value=$id></td>";
		    }
		    echo "</tr>\n";
		}
	}
        ?>
    </tbody>
</table>