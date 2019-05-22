<!--block of code that display Login and Registration fiels on one page-->
<h3>Login here</h3>
<?php
    if(($_POST) and ($_POST['logreg_button'] == "Login"))
    /*Show error messages under Login if they were Login related errors*/
    {
        echo $message;
    }
    if (($_POST) AND isset($_POST['loginemail']))
    {
        $loginemail = $_POST['loginemail'];
    }
?>
<label for="loginemail">Email:</label>
<input type="text" name="loginemail" id="loginemail" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['loginemail'].'"';}?>/><br/><br/>
<label for="loginpassword">Password:</label>
<input type="password" name="loginpassword" id="loginpassword" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['loginpassword'].'"';}?>/><br/><br/>
<a href="forgetpassword.php?loginemail=<?php if ($_POST) {echo $loginemail;} ?>">Forget Password?</a>
<p class="submit"><input type="submit" name="logreg_button" value="Login"/></p><br/><br/>
<h3>First time? Register here</h3>
<?php
    if(($_POST) and ($_POST['logreg_button'] == "Register"))
    /*Show error messages under Register if they were Register related errors*/
    {
        echo $message;
    }
?>
<label for="registeremail">Email:</label>
<input type="text" name="registeremail" id="registeremail" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['registeremail'].'"';}?>/><br/><br/>
<label for="registerpassword">Password:</label>
<input type="password" name="registerpassword" id="registerpassword" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['registerpassword'].'"';}?>/><br/><br/>
<label for="register2password">Re-enter Password:</label>
<input type="password" name="register2password" id="register2password" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['register2password'].'"';}?>/><br/><br/>
<p class="submit"><input type="submit" name="logreg_button" value="Register"/></p>
<br/><br/>