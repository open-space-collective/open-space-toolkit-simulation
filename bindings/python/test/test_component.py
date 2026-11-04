# Apache License 2.0

import pytest

from ostk.physics.coordinate import Frame

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration


@pytest.fixture
def parent_component() -> Component:
    return Component(
        id="parent-id",
        name="Parent",
        type=Component.Type.Assembly,
        tags=[],
        geometries=[],
        components=[],
        parent=None,
        frame=Frame.GCRF(),
        simulator=None,
    )


@pytest.fixture
def frameless_component() -> Component:
    return Component(
        id="camera-id",
        name="Camera",
        type=Component.Type.Sensor,
        tags=["a"],
        geometries=[],
        components=[],
        parent=None,
        frame=None,
        simulator=None,
    )


class TestComponent:
    @pytest.mark.parametrize(
        "id, name",
        [
            ("", "Camera"),
            ("camera-id", ""),
            ("", ""),
        ],
    )
    def test_constructor_with_empty_id_or_name(self, id: str, name: str):
        with pytest.raises(RuntimeError):
            Component(
                id=id,
                name=name,
                type=Component.Type.Sensor,
                tags=[],
                geometries=[],
                components=[],
                parent=None,
                frame=Frame.GCRF(),
                simulator=None,
            )

    def test_has_no_undefined(self):
        assert not hasattr(Component, "undefined")
        assert not hasattr(Component, "is_defined")

    def test_without_frame(
        self,
        frameless_component: Component,
        parent_component: Component,
    ):
        assert frameless_component.get_id() == "camera-id"
        assert frameless_component.get_name() == "Camera"
        assert frameless_component.get_type() == Component.Type.Sensor
        assert frameless_component.get_tags() == ["a"]
        assert frameless_component.get_geometries() == []

        with pytest.raises(RuntimeError):
            frameless_component.access_frame()

        parent_component.add_component(frameless_component)

        assert parent_component.has_component_with_name("Camera")

        with pytest.raises(RuntimeError):
            Component.configure(
                ComponentConfiguration(id="lens-id", name="Lens"),
                frameless_component,
            )

    def test_constructor_with_type_none(self):
        component: Component = Component(
            id="component-id",
            name="Component",
            type=None,
            tags=[],
            geometries=[],
            components=[],
            parent=None,
            frame=Frame.GCRF(),
            simulator=None,
        )

        assert component.get_type() is None

    def test_get_type(self, parent_component: Component):
        assert parent_component.get_type() == Component.Type.Assembly

    def test_configure_without_type(self, parent_component: Component):
        for configuration in (
            ComponentConfiguration(id="camera-id", name="Camera"),
            ComponentConfiguration(id="camera-id", name="Camera", type=None),
        ):
            component: Component = Component.configure(configuration, parent_component)

            assert component.get_type() is None

    def test_configure_with_type(self, parent_component: Component):
        component: Component = Component.configure(
            ComponentConfiguration(
                id="camera-id",
                name="Camera",
                type=Component.Type.Sensor,
            ),
            parent_component,
        )

        assert component.get_type() == Component.Type.Sensor

    def test_type_has_no_undefined(self):
        assert not hasattr(Component.Type, "Undefined")
        assert Component.string_from_type(Component.Type.Sensor) == "Sensor"
