<!--PHP functions that support operations needed by other pages & functions -->

<?php
	function getCurrentView($currentweek)
	{
		/*
		This function returns the pageview needed based on current week
		*/
		switch ($currentweek) {
			case 1: case 2: case 3: case 4: case 5: case 6:
				$view=0; break;
			case 7: case 8: case 9: case 10: case 11: case 12:
				$view=1; break;
			case 13: case 14: case 15: case 16: case 17: case 18:
				$view=2; break;
		}
		return $view;
	}

	function getViewParameters($view)
	{
		/*
		This function returns the parameters needed by index.php to display the Overall Summary
		*/
		switch ($view) {
			case 0:
				$previousview_id=0; $nextview_id=1; break;
			case 1:
				$previousview_id=0; $nextview_id=2; break;
			case 2:
				$previousview_id=1; $nextview_id=2; break;
		}
		$fromweek = ($view*6+1);
		$toweek = ($view+1)*6;
		return array($previousview_id, $nextview_id, $fromweek, $toweek);
	}
?>