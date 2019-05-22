<!--block of code to display the Roster fields for admin user to add participants
    to player table and teams to teams table-->
<label for="firstname">First Name:</label>
<input type="text" name="firstname" id="firstname" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['firstname'].'"';}?>/><br/><br/>
<label for="lastname">Last Name:</label>
<input type="text" name="lastname" id="lastname" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['lastname'].'"';}?>/><br/><br/>
<label for="email">Email:</label>
<input type="text" name="email" id="email" size="25" maxlength="50"<?php if ($_POST) {echo 'value="'.$_POST['email'].'"';}?>/><br/><br/>
<label for="team_id">Team Name:</label>
<select name="team_id" id="team_id">
<?php
/*display team list in drop-down list box*/
    $query="SELECT * FROM teams ORDER BY team_name ASC";
    $result=pg_exec($cxn,$query) or die ("Could not execute query");
    while ($row=pg_fetch_assoc($result))
    {
        extract($row);
        if ($_POST)
        {
            if ($_POST['team_id']==$id)
            {
                $seltext='selected="selected" ';
            }
            else
            {
                $seltext='';
            }
        }
        echo "<option ".$seltext."value=$id>$team_name</option>\n";
    }
?>
</select><br/><br/>
<label for="num_wins">#Team Wins:</label>
<input type="number" name="num_wins" id="num_wins" size="5" maxlength="2" <?php if ($_POST) {echo 'value="'.$_POST['num_wins'].'"';}?>/><br/><br/>
<input type="hidden" name="createdate" value="<?php echo $currentdate;?>">
<p class="submit"><input type="submit" name="display_button" value="Add Player"/></p>
<br/><br/>