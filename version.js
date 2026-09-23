/* מספר הגרסה של כל האתר — המקום היחיד שמעדכנים בו גרסה */
window.SITE_VERSION = "V99";
(function () {
  function apply() {
    var tags = document.querySelectorAll(".version-tag");
    for (var i = 0; i < tags.length; i++) tags[i].textContent = "גרסה " + window.SITE_VERSION;
  }
  if (document.readyState === "loading") document.addEventListener("DOMContentLoaded", apply);
  else apply();
})();
